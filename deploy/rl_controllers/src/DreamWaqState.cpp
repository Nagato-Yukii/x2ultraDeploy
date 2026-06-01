#include "rl_controllers/DreamWaqState.h"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <numeric>

#include <ament_index_cpp/get_package_share_directory.hpp>

namespace legged {

namespace {
constexpr double kTwoPi = 6.28318530717958647692;

template <typename T>
void declareIfMissing(const rclcpp_lifecycle::LifecycleNode::SharedPtr& node,
                      const std::string& name,
                      const T& default_value) {
  if (!node->has_parameter(name)) {
    node->declare_parameter<T>(name, default_value);
  }
}

template <typename T>
bool getArrayParam(const rclcpp_lifecycle::LifecycleNode::SharedPtr& node,
                   const std::string& name,
                   std::array<T, 31>& out) {
  std::vector<double> values;
  if (!node->get_parameter(name, values) || values.size() != out.size()) {
    return false;
  }
  for (std::size_t i = 0; i < out.size(); ++i) {
    out[i] = static_cast<T>(values[i]);
  }
  return true;
}

bool getIntMapParam(const rclcpp_lifecycle::LifecycleNode::SharedPtr& node,
                    const std::string& name,
                    std::array<int, 31>& out) {
  std::vector<int64_t> values;
  if (!node->get_parameter(name, values)) {
    return false;
  }
  if (values.size() != out.size()) {
    return false;
  }
  for (std::size_t i = 0; i < out.size(); ++i) {
    out[i] = static_cast<int>(values[i]);
  }
  return true;
}

std::vector<double> dreamWaqKpMujocoDefaults() {
  return {133.241870, 133.241870, 133.241870, 133.241870, 33.615754,  12.804825,  133.241870, 133.241870,
          133.241870, 133.241870, 33.615754,  12.804825,  133.241870, 12.804825,  12.804825,  33.615754,
          33.615754,  12.804825,  12.804825,  12.804825,  80.543868,  80.543868,  33.615754,  33.615754,
          12.804825,  12.804825,  12.804825,  80.543868,  80.543868,  3.947842,   3.947842};
}

std::vector<double> dreamWaqKdMujocoDefaults() {
  return {8.482441, 8.482441, 8.482441, 8.482441, 2.140045, 0.815180, 8.482441, 8.482441,
          8.482441, 8.482441, 2.140045, 0.815180, 8.482441, 0.815180, 0.815180, 2.140045,
          2.140045, 0.815180, 0.815180, 0.815180, 5.127582, 5.127582, 2.140045, 2.140045,
          0.815180, 0.815180, 0.815180, 5.127582, 5.127582, 0.251327, 0.251327};
}

float sanitizeFinite(float value, float fallback) {
  return std::isfinite(value) ? value : fallback;
}
}  // namespace

DreamWaqState::DreamWaqState()
    : memory_info_(Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault)),
      mujoco_to_isaac_idx_(identityMap()),
      isaac_to_mujoco_idx_(identityMap()),
      joint_to_actuator_idx_(identityMap()) {}

std::array<int, DreamWaqState::kActionSize> DreamWaqState::identityMap() {
  std::array<int, kActionSize> map{};
  std::iota(map.begin(), map.end(), 0);
  return map;
}

bool DreamWaqState::configure(const rclcpp_lifecycle::LifecycleNode::SharedPtr& node) {
  if (!loadCfg(node)) {
    return false;
  }
  if (!enabled_) {
    RCLCPP_INFO_STREAM(node->get_logger(), "[DreamWaQ] Disabled; skipping model load.");
    return true;
  }
  return loadModel(node);
}

bool DreamWaqState::loadCfg(const rclcpp_lifecycle::LifecycleNode::SharedPtr& node) {
  declareIfMissing<bool>(node, "DreamWaQCfg.enabled", false);

  bool cfg_enabled = false;
  node->get_parameter("DreamWaQCfg.enabled", cfg_enabled);
  enabled_ = cfg_enabled;
  if (!enabled_) {
    return true;
  }

  const std::vector<double> zero_dof(kActionSize, 0.0);
  const std::vector<double> default_kp = dreamWaqKpMujocoDefaults();
  const std::vector<double> default_kd = dreamWaqKdMujocoDefaults();
  const std::vector<int64_t> identity_map = [] {
    std::vector<int64_t> values(kActionSize);
    std::iota(values.begin(), values.end(), 0);
    return values;
  }();

  declareIfMissing<int>(node, "DreamWaQCfg.observation_size", kObservationSize);
  declareIfMissing<int>(node, "DreamWaQCfg.history_size", kHistorySize);
  declareIfMissing<int>(node, "DreamWaQCfg.action_size", kActionSize);
  declareIfMissing<int>(node, "DreamWaQCfg.decimation", decimation_);
  declareIfMissing<std::string>(node, "DreamWaQCfg.policy_file", "");
  declareIfMissing<double>(node, "DreamWaQCfg.action_scale", static_cast<double>(action_scale_));
  declareIfMissing<double>(node, "DreamWaQCfg.clip_actions", static_cast<double>(clip_actions_));
  declareIfMissing<double>(node, "DreamWaQCfg.clip_observations", static_cast<double>(clip_observations_));
  declareIfMissing<double>(node, "DreamWaQCfg.phase_period", phase_period_);
  declareIfMissing<std::vector<double>>(node, "DreamWaQCfg.default_dof_pos", zero_dof);
  declareIfMissing<std::vector<double>>(node, "DreamWaQCfg.kp", default_kp);
  declareIfMissing<std::vector<double>>(node, "DreamWaQCfg.kd", default_kd);
  declareIfMissing<std::vector<int64_t>>(node, "DreamWaQCfg.mujoco_to_isaac_idx", identity_map);
  declareIfMissing<std::vector<int64_t>>(node, "DreamWaQCfg.isaac_to_mujoco_idx", identity_map);
  declareIfMissing<std::vector<int64_t>>(node, "DreamWaQCfg.joint_to_actuator_idx", identity_map);

  int observation_size = kObservationSize;
  int history_size = kHistorySize;
  int action_size = kActionSize;
  node->get_parameter("DreamWaQCfg.observation_size", observation_size);
  node->get_parameter("DreamWaQCfg.history_size", history_size);
  node->get_parameter("DreamWaQCfg.action_size", action_size);
  node->get_parameter("DreamWaQCfg.decimation", decimation_);
  if (observation_size != kObservationSize || history_size != kHistorySize || action_size != kActionSize) {
    RCLCPP_ERROR_STREAM(node->get_logger(), "[DreamWaQ] Invalid dimensions: observation=" << observation_size
                                      << " history=" << history_size << " action=" << action_size);
    return false;
  }

  if (!node->get_parameter("DreamWaQCfg.policy_file", policy_file_path_)) {
    RCLCPP_ERROR_STREAM(node->get_logger(), "[DreamWaQ] Missing DreamWaQCfg.policy_file");
    return false;
  }

  double action_scale = action_scale_;
  double clip_actions = clip_actions_;
  double clip_observations = clip_observations_;
  node->get_parameter("DreamWaQCfg.action_scale", action_scale);
  node->get_parameter("DreamWaQCfg.clip_actions", clip_actions);
  node->get_parameter("DreamWaQCfg.clip_observations", clip_observations);
  node->get_parameter("DreamWaQCfg.phase_period", phase_period_);
  action_scale_ = static_cast<float>(action_scale);
  clip_actions_ = static_cast<float>(clip_actions);
  clip_observations_ = static_cast<float>(clip_observations);

  if (phase_period_ <= 0.0 || action_scale_ <= 0.0F || clip_actions_ <= 0.0F || clip_observations_ <= 0.0F ||
      decimation_ <= 0) {
    RCLCPP_ERROR_STREAM(node->get_logger(), "[DreamWaQ] Invalid scale/clip/phase configuration.");
    return false;
  }

  if (!getArrayParam(node, "DreamWaQCfg.default_dof_pos", default_dof_pos_mujoco_)) {
    RCLCPP_ERROR_STREAM(node->get_logger(), "[DreamWaQ] DreamWaQCfg.default_dof_pos must contain 31 MuJoCo-order floats.");
    return false;
  }

  if (!getArrayParam(node, "DreamWaQCfg.kp", kp_mujoco_)) {
    RCLCPP_ERROR_STREAM(node->get_logger(), "[DreamWaQ] DreamWaQCfg.kp must contain 31 MuJoCo-order floats.");
    return false;
  }

  if (!getArrayParam(node, "DreamWaQCfg.kd", kd_mujoco_)) {
    RCLCPP_ERROR_STREAM(node->get_logger(), "[DreamWaQ] DreamWaQCfg.kd must contain 31 MuJoCo-order floats.");
    return false;
  }

  if (!getIntMapParam(node, "DreamWaQCfg.mujoco_to_isaac_idx", mujoco_to_isaac_idx_)) {
    mujoco_to_isaac_idx_ = identityMap();
  }
  if (!getIntMapParam(node, "DreamWaQCfg.isaac_to_mujoco_idx", isaac_to_mujoco_idx_)) {
    isaac_to_mujoco_idx_ = identityMap();
  }
  if (!getIntMapParam(node, "DreamWaQCfg.joint_to_actuator_idx", joint_to_actuator_idx_)) {
    joint_to_actuator_idx_ = identityMap();
  }

  if (!validateMaps(node)) {
    return false;
  }

  return true;
}

bool DreamWaqState::validateMaps(const rclcpp_lifecycle::LifecycleNode::SharedPtr& node) const {
  auto validate_permutation = [&](const std::array<int, kActionSize>& map, const char* name) {
    std::array<bool, kActionSize> seen{};
    for (int i = 0; i < kActionSize; ++i) {
      const int value = map[i];
      if (value < 0 || value >= kActionSize) {
        RCLCPP_ERROR_STREAM(node->get_logger(), "[DreamWaQ] " << name << "[" << i << "]=" << value
                                                              << " is out of range [0, 30].");
        return false;
      }
      if (seen[value]) {
        RCLCPP_ERROR_STREAM(node->get_logger(), "[DreamWaQ] " << name << " is not a permutation; duplicate value "
                                                              << value << ".");
        return false;
      }
      seen[value] = true;
    }
    return true;
  };

  return validate_permutation(mujoco_to_isaac_idx_, "mujoco_to_isaac_idx") &&
         validate_permutation(isaac_to_mujoco_idx_, "isaac_to_mujoco_idx") &&
         validate_permutation(joint_to_actuator_idx_, "joint_to_actuator_idx");
}

bool DreamWaqState::loadModel(const rclcpp_lifecycle::LifecycleNode::SharedPtr& node) {
  const std::filesystem::path policy_path(policy_file_path_);
  const std::filesystem::path full_policy_path =
      policy_path.is_absolute()
          ? policy_path
          : std::filesystem::path(ament_index_cpp::get_package_share_directory("rl_controllers")) / policy_path;

  if (!std::filesystem::exists(full_policy_path)) {
    RCLCPP_ERROR_STREAM(node->get_logger(), "[DreamWaQ] ONNX policy file does not exist: " << full_policy_path.string());
    return false;
  }

  policy_file_path_ = full_policy_path.string();
  onnx_env_ = std::make_shared<Ort::Env>(ORT_LOGGING_LEVEL_WARNING, "DreamWaqController");
  Ort::SessionOptions session_options;
  session_options.SetInterOpNumThreads(1);
  session_ = std::make_unique<Ort::Session>(*onnx_env_, policy_file_path_.c_str(), session_options);

  input_names_.clear();
  output_names_.clear();
  input_shapes_.clear();
  output_shapes_.clear();
  input_name_storage_.clear();
  output_name_storage_.clear();

  Ort::AllocatorWithDefaultOptions allocator;
  for (int i = 0; i < static_cast<int>(session_->GetInputCount()); ++i) {
    auto name = session_->GetInputNameAllocated(i, allocator);
    input_name_storage_.push_back(std::move(name));
    input_names_.push_back(input_name_storage_.back().get());
    input_shapes_.push_back(session_->GetInputTypeInfo(i).GetTensorTypeAndShapeInfo().GetShape());
  }
  for (int i = 0; i < static_cast<int>(session_->GetOutputCount()); ++i) {
    auto name = session_->GetOutputNameAllocated(i, allocator);
    output_name_storage_.push_back(std::move(name));
    output_names_.push_back(output_name_storage_.back().get());
    output_shapes_.push_back(session_->GetOutputTypeInfo(i).GetTensorTypeAndShapeInfo().GetShape());
  }

  if (input_shapes_.empty() || output_shapes_.empty()) {
    RCLCPP_ERROR_STREAM(node->get_logger(), "[DreamWaQ] ONNX model must have at least one input and one output.");
    return false;
  }
  model_loaded_ = true;
  RCLCPP_INFO_STREAM(node->get_logger(), "[DreamWaQ] Loaded ONNX policy: " << policy_file_path_);
  return true;
}

void DreamWaqState::onEnter(const rclcpp::Time& now) {
  first_obs_ = true;
  inference_error_logged_ = false;
  invalid_value_logged_ = false;
  loop_count_ = 0;
  history_write_index_ = 0;
  phase_ = 0.0;
  last_phase_time_ = now;
  raw_action_.fill(0.0F);
  last_action_.fill(0.0F);
  clipped_action_.fill(0.0F);
  target_dof_pos_mujoco_ = default_dof_pos_mujoco_;
  target_delta_lab_.fill(0.0F);
  for (auto& frame : obs_history_) {
    frame.fill(0.0F);
  }
  obs_flat_.fill(0.0F);
}

void DreamWaqState::step(const rclcpp::Time& now,
                         const Proprioception& propri,
                         const Command& command,
                         const std::vector<std::unique_ptr<HybridJointHandle>>& joint_handles,
                         const std::vector<std::string>& joint_names,
                         const RLRobotCfg& robot_cfg) {
  if (!enabled_ || !model_loaded_) {
    RCLCPP_ERROR_STREAM(rclcpp::get_logger("rclcpp"), "[DreamWaQ] Requested but model/config is not enabled.");
    return;
  }
  if (joint_handles.size() < kActionSize || joint_names.size() < kActionSize) {
    if (!invalid_value_logged_) {
      RCLCPP_ERROR_STREAM(rclcpp::get_logger("rclcpp"),
                          "[DreamWaQ] Need 31 joint handles/names, got handles=" << joint_handles.size()
                                                                                 << " names=" << joint_names.size());
      invalid_value_logged_ = true;
    }
    return;
  }
  if (loop_count_ % decimation_ == 0) {
    updatePhase(now);
    buildObservation(propri, command, joint_handles);
    pushHistoryAndFlatten();
    runInference();
  }
  postProcessAction(joint_handles, joint_names, robot_cfg);
  ++loop_count_;
}

void DreamWaqState::updatePhase(const rclcpp::Time& now) {
  double dt = 0.0;
  if (last_phase_time_.nanoseconds() > 0) {
    dt = (now - last_phase_time_).seconds();
  }
  last_phase_time_ = now;
  if (dt < 0.0 || dt > 1.0) {
    dt = 0.0;
  }

  phase_ = std::fmod(phase_ + dt / phase_period_, 1.0);
  if (phase_ < 0.0) {
    phase_ += 1.0;
  }

  const double left = phase_;
  const double right = std::fmod(phase_ + 0.5, 1.0);
  current_obs_[102] = static_cast<float>(std::sin(kTwoPi * left));
  current_obs_[103] = static_cast<float>(std::cos(kTwoPi * left));
  current_obs_[104] = static_cast<float>(std::sin(kTwoPi * right));
  current_obs_[105] = static_cast<float>(std::cos(kTwoPi * right));
}

void DreamWaqState::buildObservation(const Proprioception& propri,
                                     const Command& command,
                                     const std::vector<std::unique_ptr<HybridJointHandle>>& joint_handles) {
  std::fill(current_obs_.begin(), current_obs_.begin() + 102, 0.0F);

  for (int i = 0; i < 3; ++i) {
    current_obs_[i] = sanitizeFinite(static_cast<float>(propri.baseAngVel(i)), 0.0F);
    current_obs_[3 + i] = sanitizeFinite(static_cast<float>(propri.projectedGravity(i)), 0.0F);
  }
  current_obs_[6] = sanitizeFinite(static_cast<float>(command.x.load()), 0.0F);
  current_obs_[7] = sanitizeFinite(static_cast<float>(command.y.load()), 0.0F);
  current_obs_[8] = sanitizeFinite(static_cast<float>(command.yaw.load()), 0.0F);

  const int joint_count = std::min<int>(kActionSize, static_cast<int>(joint_handles.size()));
  for (int lab_idx = 0; lab_idx < kActionSize; ++lab_idx) {
    const int mj_idx = mujoco_to_isaac_idx_[lab_idx];
    if (mj_idx < 0 || mj_idx >= joint_count) {
      continue;
    }
    if (!joint_handles[mj_idx]) {
      if (!invalid_value_logged_) {
        RCLCPP_ERROR_STREAM(rclcpp::get_logger("rclcpp"), "[DreamWaQ] Null joint handle at MuJoCo index " << mj_idx);
        invalid_value_logged_ = true;
      }
      continue;
    }
    const float pos = sanitizeFinite(static_cast<float>(joint_handles[mj_idx]->getPosCurr()),
                                     default_dof_pos_mujoco_[mj_idx]);
    const float vel = sanitizeFinite(static_cast<float>(joint_handles[mj_idx]->getVelCurr()), 0.0F);
    current_obs_[9 + lab_idx] =
        pos - default_dof_pos_mujoco_[mj_idx];
    current_obs_[40 + lab_idx] = vel;
  }
  for (int i = 0; i < kActionSize; ++i) {
    current_obs_[71 + i] = first_obs_ ? 0.0F : last_action_[i];
  }

  std::transform(current_obs_.begin(), current_obs_.end(), current_obs_.begin(),
                 [this](float v) { return std::clamp(sanitizeFinite(v, 0.0F), -clip_observations_, clip_observations_); });
}

void DreamWaqState::pushHistoryAndFlatten() {
  if (first_obs_) {
    for (auto& frame : obs_history_) {
      frame = current_obs_;
      std::fill(frame.begin() + 71, frame.begin() + 102, 0.0F);
    }
    first_obs_ = false;
    history_write_index_ = 0;
  } else {
    obs_history_[history_write_index_] = current_obs_;
    history_write_index_ = (history_write_index_ + 1) % kHistorySize;
  }

  std::size_t out = 0;
  for (int h = 0; h < kHistorySize; ++h) {
    const std::size_t frame_idx = (history_write_index_ + h) % kHistorySize;
    for (float v : obs_history_[frame_idx]) {
      obs_flat_[out++] = v;
    }
  }
}

void DreamWaqState::runInference() {
  try {
    const std::array<int64_t, 2> input_shape{1, kInputSize};
    std::vector<Ort::Value> input_values;
    input_values.push_back(Ort::Value::CreateTensor<float>(
        memory_info_, obs_flat_.data(), obs_flat_.size(), input_shape.data(), input_shape.size()));

    Ort::RunOptions run_options;
    std::vector<Ort::Value> output_values =
        session_->Run(run_options, input_names_.data(), input_values.data(), 1, output_names_.data(), 1);
    const auto output_info = output_values[0].GetTensorTypeAndShapeInfo();
    const std::size_t output_count = output_info.GetElementCount();
    if (output_count < kActionSize) {
      if (!inference_error_logged_) {
        RCLCPP_ERROR_STREAM(rclcpp::get_logger("rclcpp"),
                            "[DreamWaQ] ONNX output has " << output_count << " elements, expected at least "
                                                          << kActionSize << ".");
        inference_error_logged_ = true;
      }
      raw_action_.fill(0.0F);
      return;
    }

    const float* output = output_values[0].GetTensorData<float>();
    for (int i = 0; i < kActionSize; ++i) {
      raw_action_[i] = output[i];
    }
    inference_error_logged_ = false;
  } catch (const Ort::Exception& e) {
    if (!inference_error_logged_) {
      RCLCPP_ERROR_STREAM(rclcpp::get_logger("rclcpp"), "[DreamWaQ] ONNX inference failed: " << e.what());
      inference_error_logged_ = true;
    }
    raw_action_.fill(0.0F);
  }
}

void DreamWaqState::postProcessAction(const std::vector<std::unique_ptr<HybridJointHandle>>& joint_handles,
                                      const std::vector<std::string>& joint_names,
                                      const RLRobotCfg& /*robot_cfg*/) {
  for (int lab_idx = 0; lab_idx < kActionSize; ++lab_idx) {
    const float raw = raw_action_[lab_idx];
    if (!std::isfinite(raw)) {
      if (!invalid_value_logged_) {
        RCLCPP_ERROR_STREAM(rclcpp::get_logger("rclcpp"),
                            "[DreamWaQ] Non-finite ONNX action at LAB index " << lab_idx << ": " << raw);
        invalid_value_logged_ = true;
      }
    }
    const float clipped = std::clamp(sanitizeFinite(raw, 0.0F), -clip_actions_, clip_actions_);
    clipped_action_[lab_idx] = clipped;
    last_action_[lab_idx] = clipped;
    target_delta_lab_[lab_idx] = action_scale_ * clipped;
  }

  const int joint_count =
      std::min<int>(kActionSize, static_cast<int>(std::min(joint_handles.size(), joint_names.size())));
  for (int mj_idx = 0; mj_idx < joint_count; ++mj_idx) {
    const int lab_idx = isaac_to_mujoco_idx_[mj_idx];
    if (lab_idx < 0 || lab_idx >= kActionSize) {
      continue;
    }
    if (!joint_handles[mj_idx]) {
      if (!invalid_value_logged_) {
        RCLCPP_ERROR_STREAM(rclcpp::get_logger("rclcpp"), "[DreamWaQ] Null joint handle at MuJoCo index " << mj_idx);
        invalid_value_logged_ = true;
      }
      continue;
    }

    const float target = default_dof_pos_mujoco_[mj_idx] + target_delta_lab_[lab_idx];
    target_dof_pos_mujoco_[mj_idx] = target;

    joint_handles[mj_idx]->setCommand(target, 0.0, kp_mujoco_[mj_idx], kd_mujoco_[mj_idx], 0.0);
  }
}

}  // namespace legged
