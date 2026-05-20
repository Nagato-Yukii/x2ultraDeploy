#pragma once

#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <deque>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <Eigen/Dense>

#include "hardware_interface/handle.hpp"
#include "hardware_interface/hardware_info.hpp"
#include "hardware_interface/system_interface.hpp"
#include "hardware_interface/types/hardware_interface_return_values.hpp"
#include "rclcpp/clock.hpp"
#include "rclcpp/duration.hpp"
#include "rclcpp/macros.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp/time.hpp"
#include "rclcpp_lifecycle/node_interfaces/lifecycle_node_interface.hpp"
#include "rclcpp_lifecycle/state.hpp"

#include <geometry_msgs/msg/transform_stamped.hpp>
#include <tf2_ros/transform_broadcaster.h>

#include "Types.h"
#include "realtime_tools/realtime_box.hpp"
#include "realtime_tools/realtime_buffer.hpp"
#include "realtime_tools/realtime_publisher.hpp"
#include "sensor_msgs/msg/imu.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"

// for A2 (closed-chain ankle, old protocol) — commented out for X2
// #include "ClosedAnkleWristParam.h"
// #include "LoopAnkleAnalyticalSolver.h"

#include "aimdk_msgs/msg/joint_command_array.hpp"
#include "aimdk_msgs/msg/joint_state_array.hpp"

#include "aimrt_module_cpp_interface/core.h"
#include "aimrt_module_ros2_interface/channel/ros2_channel.h"
#include "core/aimrt_core.h"

namespace legged {

struct MotorData {
  double pos_, vel_, tau_;                 // state
  double posDes_, velDes_, kp_, kd_, ff_;  // command
};
struct MotorDataFloat {
  float pos_, vel_, tau_;                 // state
  float posDes_, velDes_, kp_, kd_, ff_;  // command
};//[leg,arm,wasit,head]

using Clock = std::chrono::high_resolution_clock;  // high-precision clock
using Duration = std::chrono::duration<double>;    // double-based duration



// for x2.5
/*
template <size_t Dof>
struct JointStateSample {
  std::int64_t stamp_ns = 0;
  std::array<float, Dof> pos{};
  std::array<float, Dof> vel{};
  std::array<float, Dof> tau{};
};

struct ImuSample {
  std::int64_t stamp_ns = 0;
  std::array<double, 4> orientation{};
  std::array<double, 3> angular_velocity{};
  std::array<double, 3> linear_acceleration{};
};
*/
// for x2.5 over

class LeggedSystemHardware : public hardware_interface::SystemInterface {
 public:
  hardware_interface::CallbackReturn on_init(const hardware_interface::HardwareInfo &info) override;
  std::vector<hardware_interface::StateInterface> export_state_interfaces() override;
  std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;
  hardware_interface::CallbackReturn on_activate(const rclcpp_lifecycle::State &previous_state) override;
  hardware_interface::CallbackReturn on_deactivate(const rclcpp_lifecycle::State &previous_state) override;
  hardware_interface::return_type read(const rclcpp::Time &time, const rclcpp::Duration &period) override;
  hardware_interface::return_type write(const rclcpp::Time &time, const rclcpp::Duration &period) override;

  // X2 Ultra: 12 leg + 3 waist + 14 arm + 2 head = 31 DoF
  static constexpr size_t kLegDof = 12;
  static constexpr size_t kWaistDof = 3;
  static constexpr size_t kArmDof = 14;
  static constexpr size_t kHeadDof = 2;
  static constexpr size_t kTotalDof = kLegDof + kWaistDof + kArmDof + kHeadDof;
  


 private:
  // for A2 closed-chain ankle — not needed for X2 (serial ankle joints)
  // void processClosedChainState();
  // void processClosedChainCommands();

  void aimrtInit();
  void imuCallback(const std::shared_ptr<const sensor_msgs::msg::Imu>& msg){
    sensor_msgs::msg::Imu imu_msg;
    {
      std::scoped_lock lock(motor_mtx_);
      bodyDriveIMU_ = *msg;
      imu_msg = bodyDriveIMU_;
    }
    publishImuTransform(imu_msg);
  }

  // 生成sample(含时间戳,pos,vel,tau)并做PushSample()入时序缓冲
  void legStateCallback(const std::shared_ptr<const aimdk_msgs::msg::JointStateArray>& msg)
  {
    if (msg->joints.size() < kLegDof) {
      RCLCPP_WARN_THROTTLE(rclcpp::get_logger("LeggedSystemHardware"), *node_->get_clock(), 1000,
                           "Ignore short leg state: %zu joints", msg->joints.size());
      return;
    }
    std::scoped_lock lock(motor_mtx_);
    for(size_t i = 0 ; i < kLegDof ; i++){
      //msg->joints的长度和bodyDriveJointData_不同，后者是几个msg的joints的长度的和，fix:越界读
      //原来是bodyDriveJointData_[i].pos_ = msg->joints[i].position;
      bodyDriveJointData_[i].pos_ = static_cast<float>(msg->joints[i].position);
      bodyDriveJointData_[i].vel_ = static_cast<float>(msg->joints[i].velocity);
      bodyDriveJointData_[i].tau_ = static_cast<float>(msg->joints[i].effort); 
    }
  }
  void waistStateCallback(const std::shared_ptr<const aimdk_msgs::msg::JointStateArray>& msg){
    if (msg->joints.size() < kWaistDof) {
      RCLCPP_WARN_THROTTLE(rclcpp::get_logger("LeggedSystemHardware"), *node_->get_clock(), 1000,
                           "Ignore short waist state: %zu joints", msg->joints.size());
      return;
    }
    std::scoped_lock lock(motor_mtx_);
    for(size_t i = 0 ; i < kWaistDof ; i++){
      const size_t dst = kLegDof + i;
      bodyDriveJointData_[dst].pos_ = static_cast<float>(msg->joints[i].position);
      bodyDriveJointData_[dst].vel_ = static_cast<float>(msg->joints[i].velocity);
      bodyDriveJointData_[dst].tau_ = static_cast<float>(msg->joints[i].effort);
    }
    firstReceiveWaistState = false;
  }
  void armStateCallback(const std::shared_ptr<const aimdk_msgs::msg::JointStateArray>& msg){
    if (msg->joints.size() < kArmDof) {
      RCLCPP_WARN_THROTTLE(rclcpp::get_logger("LeggedSystemHardware"), *node_->get_clock(), 1000,
                           "Ignore short arm state: %zu joints", msg->joints.size());
      return;
    }
    std::scoped_lock lock(motor_mtx_);
    for(size_t i = 0 ; i < kArmDof ; i++){
      const size_t dst = kLegDof + kWaistDof + i;
      bodyDriveJointData_[dst].pos_ = static_cast<float>(msg->joints[i].position);
      bodyDriveJointData_[dst].vel_ = static_cast<float>(msg->joints[i].velocity);
      bodyDriveJointData_[dst].tau_ = static_cast<float>(msg->joints[i].effort);
    }
    firstReceiveArmState = false;
  }

  void headStateCallback(const std::shared_ptr<const aimdk_msgs::msg::JointStateArray>& msg){
    if (msg->joints.size() < kHeadDof) {
      RCLCPP_WARN_THROTTLE(rclcpp::get_logger("LeggedSystemHardware"), *node_->get_clock(), 1000,
                           "Ignore short head state: %zu joints", msg->joints.size());
      return;
    }
    std::scoped_lock lock(motor_mtx_);
    for(size_t i = 0 ; i < kHeadDof ; i++){
      const size_t dst = kLegDof + kWaistDof + kArmDof + i;
      bodyDriveJointData_[dst].pos_ = static_cast<float>(msg->joints[i].position);
      bodyDriveJointData_[dst].vel_ = static_cast<float>(msg->joints[i].velocity);
      bodyDriveJointData_[dst].tau_ = static_cast<float>(msg->joints[i].effort);
    }
    firstReceiveHeadState = false;
  }

  void publishImuTransform(const sensor_msgs::msg::Imu& imu_msg) { // same as A2
  geometry_msgs::msg::TransformStamped transform_stamped;
  transform_stamped.header.stamp = node_->now();
  transform_stamped.header.frame_id = "base_link";
  transform_stamped.child_frame_id = "world_frame";
  transform_stamped.transform.translation.x = 0.0;
  transform_stamped.transform.translation.y = 0.0;
  transform_stamped.transform.translation.z = 0.0;
  transform_stamped.transform.rotation = imu_msg.orientation;
  tfBroadcaster_->sendTransform(transform_stamped);
}

  // 数据拷贝,将消息里的腿部关节值写入,不含时间戳,无缓冲管理
  /*
  void updateRawLegState(const aimdk_msgs::msg::JointStateArray& msg);
  void updateRawArmState(const aimdk_msgs::msg::JointStateArray& msg);
  void updateRawWaistState(const aimdk_msgs::msg::JointStateArray& msg);
  void updateRawHeadState(const aimdk_msgs::msg::JointStateArray& msg);
  void updateRawImuState(const sensor_msgs::msg::Imu& msg);

  bool fillAlignedState(std::int64_t target_ns,
                        std::array<MotorDataFloat, 31>& joint_data,
                        sensor_msgs::msg::Imu& imu_data);
  */

  MotorData SerialJointData_[kTotalDof]{};
  MotorDataFloat bodyDriveJointData_[kTotalDof]{};
  sensor_msgs::msg::Imu bodyDriveIMU_;
  double imu_orientation_[4];     
  double imu_angular_velocity_[3];
  double imu_linear_acceleration_[3];

  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr motorPosPublisher_;
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr motorVelPublisher_;
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr motorTorquePublisher_;

  // for A2 closed-chain ankle space publishers — not needed for X2
  // rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr readAnkleSpacePosPublisher_;
  // rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr readAnkleSpaceVelPublisher_;
  // rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr readAnkleSpaceTorquePublisher_;

  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr motorCmdTorquePublisher_;

  std::shared_ptr<tf2_ros::TransformBroadcaster> tfBroadcaster_;    
  std::shared_ptr<rclcpp::executors::SingleThreadedExecutor> executor_;
  std::thread executor_thread_;

  aimrt::runtime::core::AimRTCore core_;
  aimrt::runtime::core::AimRTCore::Options options_;
  aimrt::CoreRef module_handle_;
  //aimrt::channel::SubscriberRef aimrtMotorStateSubscriber_;
  aimrt::channel::SubscriberRef aimrtLegMotorStateSubscriber_;     // /aima/hal/joint/leg/state
  aimrt::channel::SubscriberRef aimrtArmMotorStateSubscriber_;  // /aima/hal/joint/arm/state
  aimrt::channel::SubscriberRef aimrtWaistStateSubscriber_;     // /aima/hal/joint/waist/state
  aimrt::channel::SubscriberRef aimrtHeadStateSubscriber_;      // /aima/hal/joint/head/state
  aimrt::channel::SubscriberRef aimrtImuSubscriber_;            // /aima/hal/imu/torso/state

  aimrt::channel::PublisherRef aimrtLegCmdPublisher_;
  aimrt::channel::PublisherRef aimrtArmCmdPublisher_;
  aimrt::channel::PublisherRef aimrtWaistCmdPublisher_;
  aimrt::channel::PublisherRef aimrtHeadCmdPublisher_;

  std::unique_ptr<aimrt::channel::PublisherProxy<aimdk_msgs::msg::JointCommandArray>> aimrtLegCmdPublisherProxy_;
  std::unique_ptr<aimrt::channel::PublisherProxy<aimdk_msgs::msg::JointCommandArray>> aimrtArmCmdPublisherProxy_;
  std::unique_ptr<aimrt::channel::PublisherProxy<aimdk_msgs::msg::JointCommandArray>> aimrtWaistCmdPublisherProxy_;
  std::unique_ptr<aimrt::channel::PublisherProxy<aimdk_msgs::msg::JointCommandArray>> aimrtHeadCmdPublisherProxy_;
  std::future<void> fu_;

  std::shared_ptr<rclcpp::Node> node_;

  std::thread loopThread_;
  std::atomic_bool loopRunning_{true};
  std::mutex motor_mtx_;
  //std::mutex state_buffer_mutex_;

  //std::deque<JointStateSample<kLegDof>> leg_state_buffer_;
  //std::deque<JointStateSample<kArmDof>> arm_state_buffer_;
  //std::deque<JointStateSample<kWaistDof>> waist_state_buffer_;
  //std::deque<JointStateSample<kHeadDof>> head_state_buffer_;
  //std::deque<ImuSample> imu_state_buffer_;
  //size_t state_buffer_capacity_{512};
  //std::int64_t state_align_delay_ns_{2'000'000};
  //std::int64_t control_cycle_stamp_ns_{0};
  //std::int64_t aligned_state_stamp_ns_{0};

  aimdk_msgs::msg::JointCommandArray legJointCommand_;
  aimdk_msgs::msg::JointCommandArray armJointCommand_;
  aimdk_msgs::msg::JointCommandArray waistJointCommand_;
  aimdk_msgs::msg::JointCommandArray headJointCommand_;
  uint32_t command_sequence_{0};

  double loopHz_{1000};

  // X2: all serial joints, no direction inversion needed; 31 joints total
  // TODO: adjust individual values if X2 hardware has reversed motor sign conventions
  std::vector<int> direction_motor = std::vector<int>(kTotalDof, 1);

  // X2: serial joints, hardware zero == URDF zero; all offsets are 0
  // TODO: fill in non-zero values if X2 calibration reveals zero-offset differences
  float base_motor[kTotalDof] = {};

  // for A2 closed-chain ankle torque mode — not needed for X2
  // bool useAnkleTorque_{true};
  bool firstReceiveWaistState{true};
  bool firstReceiveArmState{true};
  bool firstReceiveHeadState{true};
  // X2: Before/AfterConversion are identical (no closed-chain transform), kept for API compatibility
  double readPosArrayBeforeConversion_[kTotalDof]{};
  double readVelArrayBeforeConversion_[kTotalDof]{};
  double readTauArrayBeforeConversion_[kTotalDof]{};
  double readPosArrayAfterConversion_[kTotalDof]{};
  double readVelArrayAfterConversion_[kTotalDof]{};
  double readTauArrayAfterConversion_[kTotalDof]{};

  double writePosDesArray_[kTotalDof]{};
  double writeVelDesArray_[kTotalDof]{};
  double writeFFArray_[kTotalDof]{};
  double writeKpArray_[kTotalDof]{};
  double writeKdArray_[kTotalDof]{};

  double motorFFArray_[kTotalDof]{};

  vector_t motor_pos_feedback_{kTotalDof};
  vector_t motor_vel_feedback_{kTotalDof};
  vector_t motor_tau_feedback_{kTotalDof};

  // for A2 closed-chain ankle space — not needed for X2
  // vector_t read_Ankle_Space_Pos{kTotalDof};
  // vector_t read_Ankle_Space_Vel{kTotalDof};
  // vector_t read_Ankle_Space_Torque{kTotalDof};

  vector_t motor_cmd_torque{kTotalDof};

  vector_t joint_power_real_{kTotalDof};
  vector_t power_sum_real_{1};

  // for A2 closed-chain ankle solvers — not needed for X2 (serial ankle joints)
  // std::unique_ptr<zy::LoopAnkleAnalyticalSolver> ankle_state_convert;
  // std::unique_ptr<zy::LoopAnkleAnalyticalSolver> ankle_command_convert;
  // std::unique_ptr<zy::ClosedAnkleWristParam> closed_ankle_wrist_param_;
};

}  // namespace legged
