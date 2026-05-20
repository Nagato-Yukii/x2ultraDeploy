// =============================================================================
// Purpose  : ROS2 ros2_control 框架下 RL 控制器的基类。实现完整的控制循环骨架：
//            读取硬件状态接口（关节 pos/vel/effort + IMU）→ 状态估计 →
//            DEFAULT/WALK 模式状态机分发 → 调用派生类推理 →
//            将 PD 目标写回 ros2_control 命令接口。
//
// Dependencies: controller_interface（ros2_control 生命周期框架）、
//              hardware_interface（关节读写接口）、onnxruntime（策略推理）、
//              ROS2 msg 类型（geometry_msgs/Twist, sensor_msgs/Imu, Joy）；
//              被 AcController 继承；被 ros2_control 控制器管理器动态加载。
//
// Architecture: 模板方法模式（Template Method Pattern）。
//   基类 update() 定义控制循环骨架，派生类通过覆盖四个纯虚方法注入策略：
//     loadModel()         → 加载 ONNX 模型
//     loadRLCfg()         → 加载 YAML 配置（观测缩放、PD 参数等）
//     computeObservation()→ 构造策略输入观测向量
//     computeActions()    → 执行推理，将动作写入 Posdes_
//   这使得在不改变控制循环结构的情况下可以替换不同的策略网络架构。
// =============================================================================
#pragma once

#include <onnxruntime_cxx_api.h>
#include <urdf/model.h>
#include <atomic>
#include <controller_interface/controller_interface.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <hardware_interface/system_interface.hpp>
#include <hardware_interface/types/hardware_interface_type_values.hpp>
#include <map>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_lifecycle/lifecycle_node.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <sensor_msgs/msg/joy.hpp>
#include <std_msgs/msg/empty.hpp>
#include <std_msgs/msg/float32.hpp>
#include <std_msgs/msg/float64.hpp>
#include <vector>

#include "rl_controllers/HybridJointHandle.h"
#include "rl_controllers/Types.h"
#include "std_msgs/msg/float64_multi_array.hpp"

#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/msg/transform_stamped.hpp>

namespace legged {

/** 从 YAML 配置文件加载的机器人策略部署参数集合。
 *  包含 PD 增益、动作缩放系数、decimation（控制频率降频比）、
 *  初始站立姿态和观测归一化系数。
 */
struct RLRobotCfg {
  struct ControlCfg {
    std::map<std::string, float> stiffness;
    std::map<std::string, float> damping;
    float actionScale;
    int decimation;
    float user_torque_limit;
    float user_power_limit;
    //float cycle_time;
  };

  struct LegInitState {
    // leg 12)
    scalar_t left_hip_pitch_joint;
    scalar_t left_hip_roll_joint;
    scalar_t left_hip_yaw_joint;
    scalar_t left_knee_joint;
    scalar_t left_ankle_pitch_joint;
    scalar_t left_ankle_roll_joint;
    scalar_t right_hip_pitch_joint;
    scalar_t right_hip_roll_joint;
    scalar_t right_hip_yaw_joint;
    scalar_t right_knee_joint;
    scalar_t right_ankle_pitch_joint;
    scalar_t right_ankle_roll_joint;
  };
  struct WaistInitState {
    // waist (3)
    scalar_t waist_yaw_joint;
    scalar_t waist_pitch_joint;
    scalar_t waist_roll_joint;
  };
  struct ArmInitState {
    // arm (14)
    scalar_t left_shoulder_pitch_joint;
    scalar_t left_shoulder_roll_joint;
    scalar_t left_shoulder_yaw_joint;
    scalar_t left_elbow_joint;
    scalar_t left_wrist_yaw_joint;
    scalar_t left_wrist_pitch_joint;
    scalar_t left_wrist_roll_joint;
    scalar_t right_shoulder_pitch_joint;
    scalar_t right_shoulder_roll_joint;
    scalar_t right_shoulder_yaw_joint;
    scalar_t right_elbow_joint;
    scalar_t right_wrist_yaw_joint;
    scalar_t right_wrist_pitch_joint;
    scalar_t right_wrist_roll_joint;
  };
  struct HeadInitState {
    // head (2)
    scalar_t head_yaw_joint;
    scalar_t head_pitch_joint;
  };

  struct ObsScales {
    scalar_t linVel;
    scalar_t angVel;
    scalar_t dofPos;
    scalar_t dofVel;
    scalar_t quat;
    scalar_t projectedG;
    //scalar_t heightMeasurements;
  };

  bool encoder_nomalize;

  scalar_t clipActions;
  scalar_t clipObs;

  //LegInitState initState;
  LegInitState leginitState;
  WaistInitState waistinitState;
  ArmInitState arminitState;
  HeadInitState headinitState;
  ObsScales obsScales;
  ControlCfg controlCfg;
};

/** X2 Ultra 全身关节状态快照（31 个自由度）。
 *  字段顺序与硬件接口 joint_names_ 的索引一致：
 *  leg[0..11] → waist[12..14] → arm[15..28] → head[29..30]。
 */
struct JointState {
  // leg (12) — policy-controlled
  scalar_t left_hip_pitch_joint;
  scalar_t left_hip_roll_joint;
  scalar_t left_hip_yaw_joint;
  scalar_t left_knee_joint;
  scalar_t left_ankle_pitch_joint;
  scalar_t left_ankle_roll_joint;
  scalar_t right_hip_pitch_joint;
  scalar_t right_hip_roll_joint;
  scalar_t right_hip_yaw_joint;
  scalar_t right_knee_joint;
  scalar_t right_ankle_pitch_joint;
  scalar_t right_ankle_roll_joint;
  // waist (3)
  scalar_t waist_yaw_joint;
  scalar_t waist_pitch_joint;
  scalar_t waist_roll_joint;
  // arm (14)
  scalar_t left_shoulder_pitch_joint;
  scalar_t left_shoulder_roll_joint;
  scalar_t left_shoulder_yaw_joint;
  scalar_t left_elbow_joint;
  scalar_t left_wrist_yaw_joint;
  scalar_t left_wrist_pitch_joint;
  scalar_t left_wrist_roll_joint;
  scalar_t right_shoulder_pitch_joint;
  scalar_t right_shoulder_roll_joint;
  scalar_t right_shoulder_yaw_joint;
  scalar_t right_elbow_joint;
  scalar_t right_wrist_yaw_joint;
  scalar_t right_wrist_pitch_joint;
  scalar_t right_wrist_roll_joint;
  // head (2)
  scalar_t head_yaw_joint;
  scalar_t head_pitch_joint;
};

struct JoyInfo {
  float axes[8];
  int buttons[11];
};

struct Proprioception {
  vector_t jointPos;
  vector_t jointVel;
  vector3_t baseAngVel;
  vector3_t baseEulerXyz;
  //vector_t refPos;
  //vector_t posDiff;
  vector3_t projectedGravity;
};

/** 来自 /cmd_vel 或手柄的速度指令（线速度 x/y + 偏航速率）。
 *  使用 std::atomic 保证从 ROS2 订阅回调线程写入、控制循环线程读取的无锁安全。
 */
struct Command {
  std::atomic<scalar_t> x;
  std::atomic<scalar_t> y;
  std::atomic<scalar_t> yaw;
};

/** ROS2 ros2_control RL 控制器基类。
 *
 *  实现 ros2_control 生命周期（on_init → on_configure → on_activate →
 *  update（循环）→ on_deactivate → on_cleanup）。
 *  update() 是控制循环的主体，每个 ros2_control 周期（~500Hz）被调用一次，
 *  内部按 decimation 降频后执行 computeObservation() + computeActions()。
 *
 *  Mode 状态机：DEFAULT（阻尼保持当前位置）→ WALK（RL 策略主动控制）。
 *  切换由 /toggle_walk Topic 触发，或在 on_activate 时由 start_mode_name_ 决定。
 */
class RLControllerBase : public controller_interface::ControllerInterface {
 public:
  enum class Mode : uint8_t { DEFAULT, LIE, STAND, WALK };

  RLControllerBase();
  controller_interface::InterfaceConfiguration command_interface_configuration() const override;
  controller_interface::InterfaceConfiguration state_interface_configuration() const override;
  controller_interface::return_type update(const rclcpp::Time& time, const rclcpp::Duration& period) override;
  controller_interface::CallbackReturn on_init() override;
  controller_interface::CallbackReturn on_configure(const rclcpp_lifecycle::State& previous_state) override;
  controller_interface::CallbackReturn on_activate(const rclcpp_lifecycle::State& previous_state) override;
  controller_interface::CallbackReturn on_deactivate(const rclcpp_lifecycle::State& previous_state) override;
  controller_interface::CallbackReturn on_cleanup(const rclcpp_lifecycle::State& previous_state) override;
  controller_interface::CallbackReturn on_error(const rclcpp_lifecycle::State& previous_state) override;
  controller_interface::CallbackReturn on_shutdown(const rclcpp_lifecycle::State& previous_state) override;

  virtual bool loadModel() { return false; };
  virtual bool loadRLCfg() { return false; };
  virtual void computeActions(){};
  virtual void computeObservation(){};

  virtual void handleDefaultMode();
  virtual void handleWalkMode(){};
  virtual void handleLieMode();
  virtual void handleStandMode();
  virtual void onEnterWalk(){};
  virtual void starting();
  void holdJointsAtZero(const std::vector<int>& indices);
  void holdJointsAtStand(const std::vector<int>& indices);
  void holdJointsAtStandDirect(const std::vector<int>& indices);
  void holdJointsAtStandWithDeadband(const std::vector<int>& indices);
  void resetStandHoldReference();

 protected:
  //void configureStartupMode();
  virtual void updateStateEstimation(const rclcpp::Time& time, const rclcpp::Duration& period);

  virtual void cmdVelCallback(const geometry_msgs::msg::Twist::SharedPtr msg);
  virtual void joyInfoCallback(const sensor_msgs::msg::Joy::SharedPtr msg);

  std::vector<std::string> joint_names_;
  std::vector<std::string> command_interface_types_;
  std::vector<std::string> state_interface_types_;
  //std::string start_mode_name_;
  //double startup_walk_command_x_{0.0};
  //double startup_walk_command_y_{0.0};
  //double startup_walk_command_yaw_{0.0};

  bool new_msg_ = false;
  rclcpp::Time start_time_;

  /* sim !!!*/
  std::vector<std::unique_ptr<HybridJointHandle>> hybridJointHandles_;

  Mode mode_;
  //Mode startupMode_{Mode::DEFAULT};
  int64_t loopCount_;
  Command command_;
  RLRobotCfg robotCfg_{};

  // X2 Ultra standing pose (action=0 → default_joint_angle from sim2sim_details.txt sec 7)
  JointState standJointState{
    // leg
    -0.15,  // left_hip_pitch_joint
     0.0,   // left_hip_roll_joint
     0.0,   // left_hip_yaw_joint
     0.3,   // left_knee_joint
    -0.15,  // left_ankle_pitch_joint
     0.0,   // left_ankle_roll_joint
    -0.15,  // right_hip_pitch_joint
     0.0,   // right_hip_roll_joint
     0.0,   // right_hip_yaw_joint
     0.3,   // right_knee_joint
    -0.15,  // right_ankle_pitch_joint
     0.0,   // right_ankle_roll_joint
    // waist
     0.0, 0.0, 0.0,
    // arm
     0.3,  // left_shoulder_pitch_joint
     0.2,  // left_shoulder_roll_joint
     0.0,  // left_shoulder_yaw_joint
    -0.9,  // left_elbow_joint
     0.0, 0.0, 0.0,  // left wrist
     0.3,  // right_shoulder_pitch_joint
    -0.2,  // right_shoulder_roll_joint
     0.0,  // right_shoulder_yaw_joint
    -0.9,  // right_elbow_joint
     0.0, 0.0, 0.0,  // right wrist
    // head
     0.0, 0.0};

    JointState lieJointState{ // same as standJointState
    // leg
    -0.15,  // left_hip_pitch_joint
     0.0,   // left_hip_roll_joint
     0.0,   // left_hip_yaw_joint
     0.3,   // left_knee_joint
    -0.15,  // left_ankle_pitch_joint
     0.0,   // left_ankle_roll_joint
    -0.15,  // right_hip_pitch_joint
     0.0,   // right_hip_roll_joint
     0.0,   // right_hip_yaw_joint
     0.3,   // right_knee_joint
    -0.15,  // right_ankle_pitch_joint
     0.0,   // right_ankle_roll_joint
    // waist
     0.0, 0.0, 0.0,
    // arm
     0.3,  // left_shoulder_pitch_joint
     0.2,  // left_shoulder_roll_joint
     0.0,  // left_shoulder_yaw_joint
    -0.9,  // left_elbow_joint
     0.0, 0.0, 0.0,  // left wrist
     0.3,  // right_shoulder_pitch_joint
    -0.2,  // right_shoulder_roll_joint
     0.0,  // right_shoulder_yaw_joint
    -0.9,  // right_elbow_joint
     0.0, 0.0, 0.0,  // right wrist
    // head
     0.0, 0.0};
    

  JoyInfo joyInfo;
  std::atomic_bool emergency_stop{false};
  std::atomic_bool start_control{false};
  std::atomic_bool start_control_latched{false};
  std::atomic_bool switch_mode_latched{false};
  std::atomic_bool walk_mode_latched{false};
  std::atomic_bool position_control_latched{false};
  rclcpp::Time switchTime;
  std::vector<std::string> jointNames;

  vector_t rbdState_;
  vector_t measuredRbdState_;
  Proprioception propri_;
  sensor_msgs::msg::Imu imuData_;
  std::shared_ptr<tf2_ros::TransformBroadcaster> tfBroadcaster_;

  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr cmdVelSub_;
  rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr joyInfoSub_;
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr emgStopSub_;
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr startCtrlSub_;
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr walkModeSub_;
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr switchModeSub_;
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr positionCtrlSub_;

  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr realJointPosPublisher_;
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr realJointVelPublisher_;
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr realJointTorquePublisher_;
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr rlPlannedJointPosPublisher_;
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr rlPlannedJointVelPublisher_;
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr rlPlannedTorquePublisher_;

  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr realImuAngularVelPublisher_;
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr realImuLinearAccPublisher_;
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr realImuEulerXyzPulbisher_;
  rclcpp::Publisher<std_msgs::msg::Empty>::SharedPtr simResetPublisher_;

  int actuatedDofNum_;
  vector_t currentPos_;
  vector_t currentVel_;
  vector_t Posdes_;
  double cmd_norm_;

  vector_t allJointPos_;
  int actionsSize_;
  // X2 joint index layout (baseline.yaml order):
  // [0..11]  = leg (policy-controlled)
  // [12..14] = waist (fixed)
  // [15..28] = arm (fixed)
  // [29..30] = head (fixed)
  //
  // Policy joint order (training): interleaved L/R per group
  //   idx  0: L_hip_pitch   → hw 0
  //   idx  1: R_hip_pitch   → hw 6
  //   idx  2: L_hip_roll    → hw 1
  //   idx  3: R_hip_roll    → hw 7
  //   idx  4: L_hip_yaw     → hw 2
  //   idx  5: R_hip_yaw     → hw 8
  //   idx  6: L_knee        → hw 3
  //   idx  7: R_knee        → hw 9
  //   idx  8: L_ankle_pitch → hw 4
  //   idx  9: R_ankle_pitch → hw 10
  //   idx 10: L_ankle_roll  → hw 5
  //   idx 11: R_ankle_roll  → hw 11
  // joint mapping,action按照该顺序赋值给leg
  std::vector<int> leg_joint_mapping   = {0, 6, 1, 7, 2, 8, 3, 9, 4, 10, 5, 11};
  // waist + arm + head all held at default
  std::vector<int> joint_mapping_fixed = {12, 13, 14,           // waist
                                          15, 16, 17, 18, 19, 20, 21,  // left arm
                                          22, 23, 24, 25, 26, 27, 28,  // right arm
                                          29, 30};              // head
  std::vector<int> waist_joint_mapping = {12, 13, 14};
  std::vector<int> arm_head_joint_mapping = {15, 16, 17, 18, 19, 20, 21,
                                             22, 23, 24, 25, 26, 27, 28,
                                             29, 30};
  std::vector<int> arm_joint_mapping   = {};  // handled via joint_mapping_fixed,预留动作eg.正弦摆臂

 private:
  // PD stand
  std::vector<scalar_t> currentJointAngles_;
  std::vector<scalar_t> initJointAngles_;
  vector_t standJointAngles_;
  vector_t lieJointAngles_;
  vector_t standHoldRefAngles_;
  //vector_t sitJointAngles_;

  scalar_t standPercent_;
  scalar_t standDuration_;
};

}  // namespace legged
