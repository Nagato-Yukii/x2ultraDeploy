#include "rl_controllers/AcController.h"
#include <algorithm>
#include <ament_index_cpp/get_package_share_directory.hpp>
#include <filesystem>
#include "rl_controllers/RotationTools.h"

namespace legged {

namespace {
constexpr double kWalkColdStartPosTolerance = 0.03;
constexpr double kWalkColdStartVelTolerance = 0.10;
}

void AcController::handleWalkMode() {
  if (pendingWalkColdStart_) {
    const bool standPoseReached =
        ((propri_.jointPos - defaultJointAnglesActuated_).cwiseAbs().maxCoeff() < kWalkColdStartPosTolerance) &&
        (propri_.jointVel.cwiseAbs().maxCoeff() < kWalkColdStartVelTolerance);

    if (!standPoseReached) {
      for (int i = 0; i < actionsSize_; i++) {
        const int jointIndex = leg_joint_mapping[i];
        const std::string partName = jointNames[jointIndex];
        const double stiffness = robotCfg_.controlCfg.stiffness.at(partName);
        const double damping = robotCfg_.controlCfg.damping.at(partName);
        hybridJointHandles_[jointIndex]->setCommand(defaultJointAngles_(jointIndex), 0, stiffness, damping, 0);
      }
      holdJointsAtStandWithDeadband(waist_joint_mapping);
      holdJointsAtStand(arm_head_joint_mapping);
      return;
    }

    isfirstRecObs_ = true;
    lastActions_.setZero();
    proprioHistoryBuffer_.setZero();
    std::fill(actions_.begin(), actions_.end(), static_cast<tensor_element_t>(0));
    loopCount_ = 0;
    pendingWalkColdStart_ = false;
    RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"),
                       "Walk cold start armed after reset-to-stand; first policy frame will bootstrap history.");
  }

  if (loopCount_ % robotCfg_.controlCfg.decimation == 0) {
    computeObservation();
    computeActions();
    // limit action range
    scalar_t actionMin = -robotCfg_.clipActions;
    scalar_t actionMax = robotCfg_.clipActions;
    std::transform(actions_.begin(), actions_.end(), actions_.begin(),
                   [actionMin, actionMax](scalar_t value) { return std::clamp(value, actionMin, actionMax); });
  }

  // set action for the 12 policy-controlled leg joints
  for (int i = 0; i < actionsSize_; i++) {
    const int jointIndex = leg_joint_mapping[i];
    const std::string partName = jointNames[jointIndex];
    scalar_t pos_des = actions_[i] * robotCfg_.controlCfg.actionScale + defaultJointAngles_(jointIndex);
    double stiffness = robotCfg_.controlCfg.stiffness.at(partName);
    double damping = robotCfg_.controlCfg.damping.at(partName);
    hybridJointHandles_[jointIndex]->setCommand(pos_des, 0, stiffness, damping, 0);
    lastActions_(i, 0) = actions_[i];
  }

  // waist + arm + head held at default positions (not zero)
  /*
  for (int idx : joint_mapping_fixed) {
    if (idx >= static_cast<int>(hybridJointHandles_.size())) continue;
    const std::string& jname = jointNames[idx];
    double kp = robotCfg_.controlCfg.stiffness.count(jname) ? robotCfg_.controlCfg.stiffness.at(jname) : 0.0;
    double kd = robotCfg_.controlCfg.damping.count(jname) ? robotCfg_.controlCfg.damping.at(jname) : 0.0;
    hybridJointHandles_[idx]->setCommand(defaultJointAngles_(idx), 0, kp, kd, 0);
  }
    */
  holdJointsAtStandWithDeadband(waist_joint_mapping);
  holdJointsAtStand(arm_head_joint_mapping);
}

void AcController::onEnterWalk() {
  resetStandHoldReference();
  pendingWalkColdStart_ = true;
}

bool AcController::loadModel() {
  std::string policyFilePath;
  if (!get_node()->get_parameter("policyFile", policyFilePath)) {
    RCLCPP_ERROR_STREAM(rclcpp::get_logger("rlccpp"), "Get policy path fail from param server, some error occur!");
    return false;
  }

  const auto logger = rclcpp::get_logger("rlccpp");
  const std::string package_share_directory = ament_index_cpp::get_package_share_directory("rl_controllers");
  const std::filesystem::path fullPolicyFilePath = std::filesystem::path(package_share_directory) / policyFilePath;

  if (!std::filesystem::exists(fullPolicyFilePath)) {
    RCLCPP_ERROR_STREAM(logger, "ONNX policy file does not exist: " << fullPolicyFilePath.string());
    return false;
  }

  policyFilePath_ = fullPolicyFilePath.string();
  RCLCPP_INFO_STREAM(logger, "Load Onnx model from path : " << policyFilePath_);

  // create env
  onnxEnvPrt_.reset(new Ort::Env(ORT_LOGGING_LEVEL_WARNING, "LeggedOnnxController"));
  // create session
  Ort::SessionOptions sessionOptions;
  sessionOptions.SetInterOpNumThreads(1);
  sessionPtr_ = std::make_unique<Ort::Session>(*onnxEnvPrt_, policyFilePath_.c_str(), sessionOptions);
  // get input and output info
  inputNames_.clear();
  outputNames_.clear();
  inputShapes_.clear();
  outputShapes_.clear();
  Ort::AllocatorWithDefaultOptions allocator;
  RCLCPP_INFO_STREAM(logger, "ONNX input count: " << sessionPtr_->GetInputCount()
                                                  << ", output count: " << sessionPtr_->GetOutputCount());
  for (int i = 0; i < static_cast<int>(sessionPtr_->GetInputCount()); i++) {
    auto inputnamePtr = sessionPtr_->GetInputNameAllocated(i, allocator);
    inputNodeNameAllocatedStrings.push_back(std::move(inputnamePtr));
    inputNames_.push_back(inputNodeNameAllocatedStrings.back().get());
    inputShapes_.push_back(sessionPtr_->GetInputTypeInfo(i).GetTensorTypeAndShapeInfo().GetShape());
    RCLCPP_DEBUG_STREAM(logger, "Input Name [" << i << "]: " << inputNames_.back());
  }
  for (int i = 0; i < static_cast<int>(sessionPtr_->GetOutputCount()); i++) {
    auto outputnamePtr = sessionPtr_->GetOutputNameAllocated(i, allocator);
    outputNodeNameAllocatedStrings.push_back(std::move(outputnamePtr));
    outputNames_.push_back(outputNodeNameAllocatedStrings.back().get());
    outputShapes_.push_back(sessionPtr_->GetOutputTypeInfo(i).GetTensorTypeAndShapeInfo().GetShape());
    RCLCPP_DEBUG_STREAM(logger, "Output Name [" << i << "]: " << outputNames_.back());
  }

  RCLCPP_INFO_STREAM(logger, "Load Onnx model successfully !!!");
  return true;
}


bool AcController::loadRLCfg() {
  RLRobotCfg::LegInitState& legInitState = robotCfg_.leginitState;
  RLRobotCfg::ArmInitState& armInitState = robotCfg_.arminitState;
  RLRobotCfg::WaistInitState& waistInitState = robotCfg_.waistinitState;
  RLRobotCfg::HeadInitState& headInitState = robotCfg_.headinitState;
  RLRobotCfg::ControlCfg& controlCfg = robotCfg_.controlCfg;
  RLRobotCfg::ObsScales& obsScales = robotCfg_.obsScales;

  int error = 0;
  // ── Leg joint defaults (policy order: pitch/roll/yaw/knee/ankle_pitch/ankle_roll × L/R) ──
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_joint_angle.left_hip_pitch_joint",  legInitState.left_hip_pitch_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_joint_angle.left_hip_roll_joint",   legInitState.left_hip_roll_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_joint_angle.left_hip_yaw_joint",    legInitState.left_hip_yaw_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_joint_angle.left_knee_joint",       legInitState.left_knee_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_joint_angle.left_ankle_pitch_joint",legInitState.left_ankle_pitch_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_joint_angle.left_ankle_roll_joint", legInitState.left_ankle_roll_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_joint_angle.right_hip_pitch_joint", legInitState.right_hip_pitch_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_joint_angle.right_hip_roll_joint",  legInitState.right_hip_roll_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_joint_angle.right_hip_yaw_joint",   legInitState.right_hip_yaw_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_joint_angle.right_knee_joint",      legInitState.right_knee_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_joint_angle.right_ankle_pitch_joint",legInitState.right_ankle_pitch_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_joint_angle.right_ankle_roll_joint",legInitState.right_ankle_roll_joint));

  // ── Waist + arm + head defaults ──
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_waist_joint_angle.waist_yaw_joint",          waistInitState.waist_yaw_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_wasit_joint_angle.waist_pitch_joint",        waistInitState.waist_pitch_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_waist_joint_angle.waist_roll_joint",         waistInitState.waist_roll_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_arm_joint_angle.left_shoulder_pitch_joint",  armInitState.left_shoulder_pitch_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_arm_joint_angle.left_shoulder_roll_joint",   armInitState.left_shoulder_roll_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_arm_joint_angle.left_shoulder_yaw_joint",    armInitState.left_shoulder_yaw_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_arm_joint_angle.left_elbow_joint",           armInitState.left_elbow_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_arm_joint_angle.left_wrist_yaw_joint",       armInitState.left_wrist_yaw_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_arm_joint_angle.left_wrist_pitch_joint",     armInitState.left_wrist_pitch_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_arm_joint_angle.left_wrist_roll_joint",      armInitState.left_wrist_roll_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_arm_joint_angle.right_shoulder_pitch_joint", armInitState.right_shoulder_pitch_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_arm_joint_angle.right_shoulder_roll_joint",  armInitState.right_shoulder_roll_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_arm_joint_angle.right_shoulder_yaw_joint",   armInitState.right_shoulder_yaw_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_arm_joint_angle.right_elbow_joint",          armInitState.right_elbow_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_arm_joint_angle.right_wrist_yaw_joint",      armInitState.right_wrist_yaw_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_arm_joint_angle.right_wrist_pitch_joint",    armInitState.right_wrist_pitch_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_arm_joint_angle.right_wrist_roll_joint",     armInitState.right_wrist_roll_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_head_joint_angle.head_yaw_joint",            headInitState.head_yaw_joint));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.init_state.default_head_joint_angle.head_pitch_joint",          headInitState.head_pitch_joint));

  error += static_cast<int>(!get_node()->get_parameters("LeggedRobotCfg.control.stiffness", controlCfg.stiffness));
  error += static_cast<int>(!get_node()->get_parameters("LeggedRobotCfg.control.damping", controlCfg.damping));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.control.action_scale", controlCfg.actionScale));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.control.decimation", controlCfg.decimation));
  //error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.control.cycle_time", controlCfg.cycle_time));

  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.normalization.clip_scales.clip_observations", robotCfg_.clipObs));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.normalization.clip_scales.clip_actions", robotCfg_.clipActions));

  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.normalization.obs_scales.lin_vel", obsScales.linVel));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.normalization.obs_scales.ang_vel", obsScales.angVel));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.normalization.obs_scales.dof_pos", obsScales.dofPos));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.normalization.obs_scales.dof_vel", obsScales.dofVel));
  //error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.normalization.obs_scales.height_measurements", obsScales.heightMeasurements));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.normalization.obs_scales.quat", obsScales.quat));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.normalization.obs_scales.projectedG", obsScales.projectedG));

  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.size.actions_size", actionsSize_));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.size.observations_size", observationSize_));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.size.num_hist", numHist_));

  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.mode.sw_mode", sw_mode_));
  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.mode.cmd_threshold", cmd_threshold_));

  error += static_cast<int>(!get_node()->get_parameter("LeggedRobotCfg.mode.stacking_mode, for observation", stacking_mode_));

  actions_.resize(actionsSize_);
  // observations_ holds one frame; history stacking is done by aimrl_sdk.
  // observationSize_ = 45
  observations_.resize(observationSize_ * numHist_);

  command_.x = 0;
  command_.y = 0;
  command_.yaw = 0;
  baseLinVel_.setZero();
  basePosition_.setZero();

  // X2 Ultra 31-DoF default angles (baseline.yaml order)
  std::vector<scalar_t> defaultJointAngles{
    // leg (0..11) — policy order: L{pitch,roll,yaw,knee,ank_p,ank_r}, R{pitch,roll,yaw,knee,ank_p,ank_r}
    legInitState.left_hip_pitch_joint,   legInitState.left_hip_roll_joint,
    legInitState.left_hip_yaw_joint,     legInitState.left_knee_joint,
    legInitState.left_ankle_pitch_joint, legInitState.left_ankle_roll_joint,
    legInitState.right_hip_pitch_joint,  legInitState.right_hip_roll_joint,
    legInitState.right_hip_yaw_joint,    legInitState.right_knee_joint,
    legInitState.right_ankle_pitch_joint,legInitState.right_ankle_roll_joint,
    // waist (12..14)
    waistInitState.waist_yaw_joint, waistInitState.waist_pitch_joint, waistInitState.waist_roll_joint,
    // arm (15..28)
    armInitState.left_shoulder_pitch_joint, armInitState.left_shoulder_roll_joint,
    armInitState.left_shoulder_yaw_joint,   armInitState.left_elbow_joint,
    armInitState.left_wrist_yaw_joint,      armInitState.left_wrist_pitch_joint,
    armInitState.left_wrist_roll_joint,
    armInitState.right_shoulder_pitch_joint,armInitState.right_shoulder_roll_joint,
    armInitState.right_shoulder_yaw_joint,  armInitState.right_elbow_joint,
    armInitState.right_wrist_yaw_joint,     armInitState.right_wrist_pitch_joint,
    armInitState.right_wrist_roll_joint,
    // head (29..30)
    headInitState.head_yaw_joint, headInitState.head_pitch_joint};
  lastActions_.resize(actionsSize_);
  lastActions_.setZero();

  const int inputSize = numHist_ * observationSize_;
  proprioHistoryBuffer_.resize(inputSize);
  proprioHistoryBuffer_.setZero();
  defaultJointAngles_.resize(actuatedDofNum_);
  defaultJointAnglesActuated_.resize(actionsSize_);

  for (int i = 0; i < actuatedDofNum_; i++) {
    defaultJointAngles_(i) = defaultJointAngles[i];
  }
  for (int i = 0; i < actionsSize_; i++) {
    defaultJointAnglesActuated_(i) = defaultJointAngles[leg_joint_mapping[i]];
  }

  return (error == 0);
}

void AcController::computeActions() {
  // create input tensor object
  std::vector<Ort::Value> inputValues;
  inputValues.push_back(Ort::Value::CreateTensor<tensor_element_t>(memoryInfo, observations_.data(), observations_.size(),
                                                                   inputShapes_[0].data(), inputShapes_[0].size()));
  // run inference
  Ort::RunOptions runOptions;
  std::vector<Ort::Value> outputValues = sessionPtr_->Run(runOptions, inputNames_.data(), inputValues.data(), 1, outputNames_.data(), 1);

  for (int i = 0; i < actionsSize_; i++) {
    actions_[i] = *(outputValues[0].GetTensorMutableData<tensor_element_t>() + i);
  }
}

void AcController::computeObservation() {
  // velocity command = [vx, vy, wz]
  vector_t velocityCommand(3);

  if (sw_mode_) {
    cmd_norm_ = std::sqrt(command_.x * command_.x + command_.y * command_.y + command_.yaw * command_.yaw);
  }

  velocityCommand[0] = command_.x;
  velocityCommand[1] = command_.y;
  velocityCommand[2] = command_.yaw;

  vector_t actions(lastActions_);
  RLRobotCfg::ObsScales& obsScales = robotCfg_.obsScales;
  vector_t proprioObs(observationSize_);

  // Observation layout must match the policy training pipeline:
  // [base_ang_vel(3), projected_gravity(3), velocity_commands(3),
  //  joint_pos_rel(12), joint_vel_rel(12), last_action(12)]
  proprioObs << propri_.baseAngVel * obsScales.angVel,                      // 3
      propri_.projectedGravity * obsScales.projectedG,                      // 3
      velocityCommand,                                                      // 3
      (propri_.jointPos - defaultJointAnglesActuated_) * obsScales.dofPos,  // 12
      propri_.jointVel * obsScales.dofVel,                                  // 12
      actions;                                                              // 12

  if (isfirstRecObs_) {
    const int actionOffset = 3 + 3 + 3 + actionsSize_ + actionsSize_;
    const int actionCount = actionsSize_;
    for (int i = 0; i < actionCount; ++i) {
      const int obsIndex = actionOffset + i;
      if (obsIndex < proprioObs.rows()) {
        proprioObs(obsIndex, 0) = 0.0;
      }
    }

    for (int i = 0; i < numHist_; i++) {
      proprioHistoryBuffer_.segment(i * observationSize_, observationSize_) = proprioObs.cast<tensor_element_t>();
    }
    isfirstRecObs_ = false;
  }

  proprioHistoryBuffer_.head(proprioHistoryBuffer_.size() - observationSize_) =
      proprioHistoryBuffer_.tail(proprioHistoryBuffer_.size() - observationSize_);
  proprioHistoryBuffer_.tail(observationSize_) = proprioObs.cast<tensor_element_t>();

  if (stacking_mode_ == "tile"){
    for (int i = 0; i < (observationSize_ * numHist_); i++) {
      observations_[i] = static_cast<tensor_element_t>(proprioHistoryBuffer_[i]);
  }
}
  else if (stacking_mode_ == "term"){
    // 动态根据各特征的变量 size 和名称设置，无需硬编码累加数字
    std::vector<std::pair<std::string, int>> obs_term_sizes = {
        {"base_ang_vel", 3},
        {"projected_gravity", 3},
        {"velocity_commands", velocityCommand.size()},
        {"joint_pos_rel", actionsSize_},
        {"joint_vel_rel", actionsSize_},
        {"last_action", actionsSize_}
    };
    // 自动计算起始索引和长度
    std::vector<std::pair<int, int>> term_specs;
    int current_offset = 0;
    for (const auto& term : obs_term_sizes) {
      term_specs.push_back({current_offset, term.second});
      current_offset += term.second;
    }
    int idx = 0;
    // 1. 遍历每一类“簇”(term)
    for (const auto& spec : term_specs) {
      int term_start = spec.first;
      int term_len = spec.second;
      
      // 2. 遍历历史时间步 (最老帧 -> 最新帧)
      for (int t = 0; t < numHist_; t++) {
        // 3. 遍历簇内的具体维度 (如: 12维关节中的第 0~11 维)
        for (int i = 0; i < term_len; i++) {
          int buffer_idx = t * observationSize_ + term_start + i;
          observations_[idx++] = static_cast<tensor_element_t>(proprioHistoryBuffer_[buffer_idx]);
        }
      }
    }
  }
  else{RCLCPP_ERROR_STREAM(rclcpp::get_logger("rlccpp"), "Unknown stacking method! Please check yaml for stacking_mode");}


  // ── Clip observations ────────────────────────────────────────────────────
  scalar_t obsMin = -robotCfg_.clipObs;
  scalar_t obsMax =  robotCfg_.clipObs;
  std::transform(observations_.begin(), observations_.end(), observations_.begin(),
                 [obsMin, obsMax](scalar_t x) { return std::max(obsMin, std::min(obsMax, x)); });
}

}  // namespace legged

#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(
    legged::AcController, controller_interface::ControllerInterface)
