#pragma once

#include "rl_controllers/RLControllerBase.h"

#include <array>
#include <memory>
#include <string>
#include <vector>

#include <onnxruntime_cxx_api.h>

namespace legged {

class DreamWaqState {
 public:
  DreamWaqState();

  bool configure(const rclcpp_lifecycle::LifecycleNode::SharedPtr& node);
  void onEnter(const rclcpp::Time& now);
  void step(const rclcpp::Time& now,
            const Proprioception& propri,
            const Command& command,
            const std::vector<std::unique_ptr<HybridJointHandle>>& joint_handles,
            const std::vector<std::string>& joint_names,
            const RLRobotCfg& robot_cfg);

  bool enabled() const {
    return enabled_;
  }

 private:
  static constexpr int kObservationSize = 106;
  static constexpr int kHistorySize = 5;
  static constexpr int kInputSize = kObservationSize * kHistorySize;
  static constexpr int kActionSize = 31;

  bool loadModel(const rclcpp_lifecycle::LifecycleNode::SharedPtr& node);
  bool loadCfg(const rclcpp_lifecycle::LifecycleNode::SharedPtr& node);
  bool validateMaps(const rclcpp_lifecycle::LifecycleNode::SharedPtr& node) const;
  void updatePhase(const rclcpp::Time& now);
  void buildObservation(const Proprioception& propri,
                        const Command& command,
                        const std::vector<std::unique_ptr<HybridJointHandle>>& joint_handles);
  void pushHistoryAndFlatten();
  void runInference();
  void postProcessAction(const std::vector<std::unique_ptr<HybridJointHandle>>& joint_handles,
                         const std::vector<std::string>& joint_names,
                         const RLRobotCfg& robot_cfg);
  static std::array<int, kActionSize> identityMap();

  bool enabled_{false};
  bool model_loaded_{false};
  bool first_obs_{true};
  bool inference_error_logged_{false};
  bool invalid_value_logged_{false};

  std::string policy_file_path_;
  std::shared_ptr<Ort::Env> onnx_env_;
  std::unique_ptr<Ort::Session> session_;
  std::vector<const char*> input_names_;
  std::vector<const char*> output_names_;
  std::vector<Ort::AllocatedStringPtr> input_name_storage_;
  std::vector<Ort::AllocatedStringPtr> output_name_storage_;
  std::vector<std::vector<int64_t>> input_shapes_;
  std::vector<std::vector<int64_t>> output_shapes_;
  Ort::MemoryInfo memory_info_;

  std::array<float, kObservationSize> current_obs_{};
  std::array<std::array<float, kObservationSize>, kHistorySize> obs_history_{};
  std::array<float, kInputSize> obs_flat_{};
  std::size_t history_write_index_{0};

  std::array<float, kActionSize> raw_action_{};
  std::array<float, kActionSize> last_action_{};
  std::array<float, kActionSize> clipped_action_{};
  std::array<float, kActionSize> default_dof_pos_mujoco_{};
  std::array<float, kActionSize> kp_mujoco_{};
  std::array<float, kActionSize> kd_mujoco_{};
  std::array<float, kActionSize> target_delta_lab_{};
  std::array<float, kActionSize> target_dof_pos_mujoco_{};
  std::array<int, kActionSize> mujoco_to_isaac_idx_{};
  std::array<int, kActionSize> isaac_to_mujoco_idx_{};
  std::array<int, kActionSize> joint_to_actuator_idx_{};

  float action_scale_{0.25F};
  float clip_actions_{100.0F};
  float clip_observations_{100.0F};
  int decimation_{10};
  int loop_count_{0};
  double phase_{0.0};
  double phase_period_{1.0};
  rclcpp::Time last_phase_time_;
};

}  // namespace legged
