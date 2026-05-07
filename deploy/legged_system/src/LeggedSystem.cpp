#include "LeggedSystem.h"

#include <ifaddrs.h>
#include <net/if.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <limits>
#include <memory>
#include <ostream>
#include <vector>

#include <Eigen/Geometry>

#include "hardware_interface/sensor_interface.hpp"
#include "hardware_interface/types/hardware_interface_type_values.hpp"
#include "rclcpp/rclcpp.hpp"

#include "Utilities.h"

namespace legged {

namespace {
// names passed to /aima/hal/joint/leg/command
constexpr std::array<const char*, LeggedSystemHardware::kLegDof> kLegJointNames = {
    "left_hip_pitch_joint",
    "left_hip_roll_joint",
    "left_hip_yaw_joint",
    "left_knee_joint",
    "left_ankle_pitch_joint",
    "left_ankle_roll_joint",
    "right_hip_pitch_joint",
    "right_hip_roll_joint",
    "right_hip_yaw_joint",
    "right_knee_joint",
    "right_ankle_pitch_joint",
    "right_ankle_roll_joint"};

// names passed to /aima/hal/joint/arm/command
constexpr std::array<const char*, LeggedSystemHardware::kArmDof> kArmJointNames = {
    "left_shoulder_pitch_joint",
    "left_shoulder_roll_joint",
    "left_shoulder_yaw_joint",
    "left_elbow_joint",
    "left_wrist_yaw_joint",
    "left_wrist_pitch_joint",
    "left_wrist_roll_joint",
    "right_shoulder_pitch_joint",
    "right_shoulder_roll_joint",
    "right_shoulder_yaw_joint",
    "right_elbow_joint",
    "right_wrist_yaw_joint",
    "right_wrist_pitch_joint",
    "right_wrist_roll_joint"};

// names passed to /aima/hal/joint/waist/command  (X2 Ultra new)
constexpr std::array<const char*, LeggedSystemHardware::kWaistDof> kWaistJointNames = {
    "waist_yaw_joint",
    "waist_pitch_joint",
    "waist_roll_joint"};

// names passed to /aima/hal/joint/head/command  (X2 Ultra new)
constexpr std::array<const char*,  LeggedSystemHardware::kHeadDof> kHeadJointNames = {
    "head_yaw_joint",
    "head_pitch_joint"};



/*
std::int64_t NowSystemNs() {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

std::int64_t TimeToNs(const builtin_interfaces::msg::Time& stamp) {
  return static_cast<std::int64_t>(stamp.sec) * 1000000000LL +
         static_cast<std::int64_t>(stamp.nanosec);
}

std::int64_t ResolveStampNs(const aimdk_msgs::msg::MessageHeader& header, std::int64_t fallback_ns) {
  const auto stamp_ns = TimeToNs(header.stamp);
  if (stamp_ns > 0) return stamp_ns;

  const auto meas_stamp_ns = TimeToNs(header.meas_stamp);
  if (meas_stamp_ns > 0) return meas_stamp_ns;

  return fallback_ns;
}

std::int64_t ResolveStampNs(const std_msgs::msg::Header& header, std::int64_t fallback_ns) {
  const auto stamp_ns = TimeToNs(header.stamp);
  return stamp_ns > 0 ? stamp_ns : fallback_ns;
}*/
/*
template <typename SampleT>
void PushSample(std::deque<SampleT>& buffer, SampleT sample, size_t capacity) {
  if (capacity == 0) return;
  if (buffer.size() >= capacity) {
    buffer.pop_front();
  }
  buffer.emplace_back(std::move(sample));
}

template <typename SampleT, typename InterpolateFn>
bool SampleAtTimestamp(const std::deque<SampleT>& buffer,
                       std::int64_t target_ns,
                       SampleT& out,
                       InterpolateFn interpolate_fn) {
  if (buffer.empty()) return false;

  if (buffer.size() == 1 || target_ns <= buffer.front().stamp_ns) {
    out = buffer.front();
    return true;
  }
  if (target_ns >= buffer.back().stamp_ns) {
    out = buffer.back();
    return true;
  }

  auto upper = std::lower_bound(
      buffer.begin(), buffer.end(), target_ns,
      [](const SampleT& sample, std::int64_t value) { return sample.stamp_ns < value; });
  if (upper == buffer.end()) {
    out = buffer.back();
    return true;
  }
  if (upper->stamp_ns == target_ns) {
    out = *upper;
    return true;
  }

  const auto lower = std::prev(upper);
  const auto duration_ns = upper->stamp_ns - lower->stamp_ns;
  if (duration_ns <= 0) {
    out = *upper;
    return true;
  }

  const double alpha = static_cast<double>(target_ns - lower->stamp_ns) /
                       static_cast<double>(duration_ns);
  interpolate_fn(*lower, *upper, alpha, out);
  out.stamp_ns = target_ns;
  return true;
}

template <size_t Dof>
void InterpolateJointSample(const JointStateSample<Dof>& lhs,
                            const JointStateSample<Dof>& rhs,
                            double alpha,
                            JointStateSample<Dof>& out) {
  for (size_t i = 0; i < Dof; ++i) {
    out.pos[i] = static_cast<float>((1.0 - alpha) * lhs.pos[i] + alpha * rhs.pos[i]);
    out.vel[i] = static_cast<float>((1.0 - alpha) * lhs.vel[i] + alpha * rhs.vel[i]);
    out.tau[i] = static_cast<float>((1.0 - alpha) * lhs.tau[i] + alpha * rhs.tau[i]);
  }
}

void InterpolateImuSample(const ImuSample& lhs,
                          const ImuSample& rhs,
                          double alpha,
                          ImuSample& out) {
  Eigen::Quaterniond q0(lhs.orientation[3], lhs.orientation[0], lhs.orientation[1], lhs.orientation[2]);
  Eigen::Quaterniond q1(rhs.orientation[3], rhs.orientation[0], rhs.orientation[1], rhs.orientation[2]);
  q0.normalize();
  q1.normalize();
  const Eigen::Quaterniond qi = q0.slerp(alpha, q1);

  out.orientation = {qi.x(), qi.y(), qi.z(), qi.w()};
  for (size_t i = 0; i < 3; ++i) {
    out.angular_velocity[i] =
        (1.0 - alpha) * lhs.angular_velocity[i] + alpha * rhs.angular_velocity[i];
    out.linear_acceleration[i] =
        (1.0 - alpha) * lhs.linear_acceleration[i] + alpha * rhs.linear_acceleration[i];
  }
}*/
}  // namespace

template <int row_>
using Vector = Eigen::Matrix<double, row_, 1>;

Vector<LeggedSystemHardware::kTotalDof> m_q;  // motor position feedback — X2 Ultra 31 DoF
Vector<LeggedSystemHardware::kTotalDof> m_v;  // motor velocity feedback
Vector<LeggedSystemHardware::kTotalDof> m_t;  // motor torque feedback

// processClosedChainState() and processClosedChainCommands() were removed

hardware_interface::CallbackReturn LeggedSystemHardware::on_init(const hardware_interface::HardwareInfo& info) {
  // Urdf not found in hardware info, error
  if (hardware_interface::SystemInterface::on_init(info) != hardware_interface::CallbackReturn::SUCCESS) {
    return hardware_interface::CallbackReturn::ERROR;
  }

  for (auto joint : info.joints) {
    RCLCPP_INFO(rclcpp::get_logger("LeggedSystemHardware"), "Found %s successfully!", joint.name.c_str());
  }

  motor_pos_feedback_.setZero();
  motor_vel_feedback_.setZero();
  motor_tau_feedback_.setZero();

  motor_cmd_torque.setZero();
  legJointCommand_.joints.resize(LeggedSystemHardware::kLegDof);
  waistJointCommand_.joints.resize(LeggedSystemHardware::kWaistDof);
  armJointCommand_.joints.resize(LeggedSystemHardware::kArmDof);
  headJointCommand_.joints.resize(LeggedSystemHardware::kHeadDof);

  //for (size_t i = 0; i < kLegJointNames.size(); i++) {
  for (size_t i = 0; i < LeggedSystemHardware::kLegDof; i++) {
    legJointCommand_.joints[i].name = kLegJointNames[i];
  }
  for (size_t i = 0; i < LeggedSystemHardware::kWaistDof; i++) {
    waistJointCommand_.joints[i].name = kWaistJointNames[i];
  }
  for (size_t i = 0; i < LeggedSystemHardware::kArmDof; i++) {
    armJointCommand_.joints[i].name = kArmJointNames[i];
  }

  for (size_t i = 0; i < LeggedSystemHardware::kHeadDof; i++) {
    headJointCommand_.joints[i].name = kHeadJointNames[i];
  }

  return hardware_interface::CallbackReturn::SUCCESS;
}


/*
void LeggedSystemHardware::updateRawLegState(const aimdk_msgs::msg::JointStateArray& msg) {
  for (size_t i = 0; i < kLegDof; ++i) {
    bodyDriveJointData_[i].pos_ = static_cast<float>(msg.joints[i].position);
    bodyDriveJointData_[i].vel_ = static_cast<float>(msg.joints[i].velocity);
    bodyDriveJointData_[i].tau_ = static_cast<float>(msg.joints[i].effort);
  }
}

void LeggedSystemHardware::updateRawWaistState(const aimdk_msgs::msg::JointStateArray& msg) {
  for (size_t i = 0; i < kWaistDof; ++i) {
    bodyDriveJointData_[kLegDof + i].pos_ = static_cast<float>(msg.joints[i].position);
    bodyDriveJointData_[kLegDof + i].vel_ = static_cast<float>(msg.joints[i].velocity);
    bodyDriveJointData_[12 + i].tau_ = static_cast<float>(msg.joints[i].effort);
  }
}

void LeggedSystemHardware::updateRawArmState(const aimdk_msgs::msg::JointStateArray& msg) {
  for (size_t i = 0; i < kArmDof; ++i) {
    bodyDriveJointData_[kLegDof + kWaistDof + i].pos_ = static_cast<float>(msg.joints[i].position);
    bodyDriveJointData_[kLegDof + kWaistDof + i].vel_ = static_cast<float>(msg.joints[i].velocity);
    bodyDriveJointData_[kLegDof + kWaistDof + i].tau_ = static_cast<float>(msg.joints[i].effort);
  }
}

void LeggedSystemHardware::updateRawHeadState(const aimdk_msgs::msg::JointStateArray& msg) {
  for (size_t i = 0; i < kHeadDof; ++i) {
    bodyDriveJointData_[kLegDof + kWaistDof + kArmDof + i].pos_ = static_cast<float>(msg.joints[i].position);
    bodyDriveJointData_[kLegDof + kWaistDof + kArmDof + i].vel_ = static_cast<float>(msg.joints[i].velocity);
    bodyDriveJointData_[kLegDof + kWaistDof + kArmDof + i].tau_ = static_cast<float>(msg.joints[i].effort);
  }
}

void LeggedSystemHardware::updateRawImuState(const sensor_msgs::msg::Imu& msg) {
  bodyDriveIMU_ = msg;
}

void LeggedSystemHardware::legStateCallback(const std::shared_ptr<const aimdk_msgs::msg::JointStateArray>& msg) {
  const auto recv_stamp_ns = NowSystemNs();
  JointStateSample<kLegDof> sample;
  sample.stamp_ns = ResolveStampNs(msg->header, recv_stamp_ns);
  for (size_t i = 0; i < kLegDof; ++i) {
    sample.pos[i] = static_cast<float>(msg->joints[i].position);
    sample.vel[i] = static_cast<float>(msg->joints[i].velocity);
    sample.tau[i] = static_cast<float>(msg->joints[i].effort);
  }

  std::scoped_lock lock(state_buffer_mutex_);
  updateRawLegState(*msg);
  PushSample(leg_state_buffer_, std::move(sample), state_buffer_capacity_);
}

void LeggedSystemHardware::armStateCallback(const std::shared_ptr<const aimdk_msgs::msg::JointStateArray>& msg) {
  const auto recv_stamp_ns = NowSystemNs();
  JointStateSample<kArmDof> sample;
  sample.stamp_ns = ResolveStampNs(msg->header, recv_stamp_ns);
  for (size_t i = 0; i < kArmDof; ++i) {
    sample.pos[i] = static_cast<float>(msg->joints[i].position);
    sample.vel[i] = static_cast<float>(msg->joints[i].velocity);
    sample.tau[i] = static_cast<float>(msg->joints[i].effort);
  }

  std::scoped_lock lock(state_buffer_mutex_);
  updateRawArmState(*msg);
  PushSample(arm_state_buffer_, std::move(sample), state_buffer_capacity_);
}

void LeggedSystemHardware::waistStateCallback(const std::shared_ptr<const aimdk_msgs::msg::JointStateArray>& msg) {
  const auto recv_stamp_ns = NowSystemNs();
  JointStateSample<kWaistDof> sample;
  sample.stamp_ns = ResolveStampNs(msg->header, recv_stamp_ns);
  for (size_t i = 0; i < kWaistDof; ++i) {
    sample.pos[i] = static_cast<float>(msg->joints[i].position);
    sample.vel[i] = static_cast<float>(msg->joints[i].velocity);
    sample.tau[i] = static_cast<float>(msg->joints[i].effort);
  }

  std::scoped_lock lock(state_buffer_mutex_);
  updateRawWaistState(*msg);
  PushSample(waist_state_buffer_, std::move(sample), state_buffer_capacity_);
}

void LeggedSystemHardware::headStateCallback(const std::shared_ptr<const aimdk_msgs::msg::JointStateArray>& msg) {
  const auto recv_stamp_ns = NowSystemNs();
  JointStateSample<kHeadDof> sample;
  sample.stamp_ns = ResolveStampNs(msg->header, recv_stamp_ns);
  for (size_t i = 0; i < kHeadDof; ++i) {
    sample.pos[i] = static_cast<float>(msg->joints[i].position);
    sample.vel[i] = static_cast<float>(msg->joints[i].velocity);
    sample.tau[i] = static_cast<float>(msg->joints[i].effort);
  }

  std::scoped_lock lock(state_buffer_mutex_);
  updateRawHeadState(*msg);
  PushSample(head_state_buffer_, std::move(sample), state_buffer_capacity_);
}

void LeggedSystemHardware::imuCallback(const std::shared_ptr<const sensor_msgs::msg::Imu>& msg) {
  const auto recv_stamp_ns = NowSystemNs();
  ImuSample sample;
  sample.stamp_ns = ResolveStampNs(msg->header, recv_stamp_ns);
  sample.orientation = {msg->orientation.x, msg->orientation.y, msg->orientation.z, msg->orientation.w};
  sample.angular_velocity = {msg->angular_velocity.x, msg->angular_velocity.y, msg->angular_velocity.z};
  sample.linear_acceleration = {msg->linear_acceleration.x, msg->linear_acceleration.y, msg->linear_acceleration.z};

  {
    std::scoped_lock lock(state_buffer_mutex_);
    updateRawImuState(*msg);
    PushSample(imu_state_buffer_, std::move(sample), state_buffer_capacity_);
  }

  publishImuTransform(*msg);
}

bool LeggedSystemHardware::fillAlignedState(std::int64_t target_ns,
                                            std::array<MotorDataFloat, 31>& joint_data,
                                            sensor_msgs::msg::Imu& imu_data) {
  bool all_streams_ready = true;

  JointStateSample<kLegDof> leg_sample;
  if (SampleAtTimestamp(leg_state_buffer_, target_ns, leg_sample, InterpolateJointSample<kLegDof>)) {
    for (size_t i = 0; i < kLegDof; ++i) {
      joint_data[i].pos_ = leg_sample.pos[i];
      joint_data[i].vel_ = leg_sample.vel[i];
      joint_data[i].tau_ = leg_sample.tau[i];
    }
  } else {
    all_streams_ready = false;
  }

  JointStateSample<kWaistDof> waist_sample;
  if (SampleAtTimestamp(waist_state_buffer_, target_ns, waist_sample, InterpolateJointSample<kWaistDof>)) {
    for (size_t i = 0; i < kWaistDof; ++i) {
      joint_data[12 + i].pos_ = waist_sample.pos[i];
      joint_data[12 + i].vel_ = waist_sample.vel[i];
      joint_data[12 + i].tau_ = waist_sample.tau[i];
    }
  } else {
    all_streams_ready = false;
  }

  JointStateSample<kArmDof> arm_sample;
  if (SampleAtTimestamp(arm_state_buffer_, target_ns, arm_sample, InterpolateJointSample<kArmDof>)) {
    for (size_t i = 0; i < kArmDof; ++i) {
      joint_data[15 + i].pos_ = arm_sample.pos[i];
      joint_data[15 + i].vel_ = arm_sample.vel[i];
      joint_data[15 + i].tau_ = arm_sample.tau[i];
    }
  } else {
    all_streams_ready = false;
  }

  JointStateSample<kHeadDof> head_sample;
  if (SampleAtTimestamp(head_state_buffer_, target_ns, head_sample, InterpolateJointSample<kHeadDof>)) {
    for (size_t i = 0; i < kHeadDof; ++i) {
      joint_data[29 + i].pos_ = head_sample.pos[i];
      joint_data[29 + i].vel_ = head_sample.vel[i];
      joint_data[29 + i].tau_ = head_sample.tau[i];
    }
  } else {
    all_streams_ready = false;
  }

  ImuSample imu_sample;
  if (SampleAtTimestamp(imu_state_buffer_, target_ns, imu_sample, InterpolateImuSample)) {
    imu_data.header.stamp.sec = static_cast<int32_t>(target_ns / 1000000000LL);
    imu_data.header.stamp.nanosec = static_cast<uint32_t>(target_ns % 1000000000LL);
    imu_data.orientation.x = imu_sample.orientation[0];
    imu_data.orientation.y = imu_sample.orientation[1];
    imu_data.orientation.z = imu_sample.orientation[2];
    imu_data.orientation.w = imu_sample.orientation[3];
    imu_data.angular_velocity.x = imu_sample.angular_velocity[0];
    imu_data.angular_velocity.y = imu_sample.angular_velocity[1];
    imu_data.angular_velocity.z = imu_sample.angular_velocity[2];
    imu_data.linear_acceleration.x = imu_sample.linear_acceleration[0];
    imu_data.linear_acceleration.y = imu_sample.linear_acceleration[1];
    imu_data.linear_acceleration.z = imu_sample.linear_acceleration[2];
  } else {
    all_streams_ready = false;
  }

  return all_streams_ready;
}
*/

// 硬件内部变量到处为ros2controle可读的状态接口，每个关节的三类状态+imu的十个状态(xyzw,wel_xyz,linear_acc_xyz)
std::vector<hardware_interface::StateInterface> LeggedSystemHardware::export_state_interfaces() {
  std::vector<hardware_interface::StateInterface> state_interfaces;
  // binding state interfaces to SerialJointData_
  for (size_t i = 0; i < info_.joints.size(); i++) {
    state_interfaces.emplace_back(hardware_interface::StateInterface(
        info_.joints[i].name, hardware_interface::HW_IF_POSITION, &SerialJointData_[i].pos_));
    state_interfaces.emplace_back(hardware_interface::StateInterface(
        info_.joints[i].name, hardware_interface::HW_IF_VELOCITY, &SerialJointData_[i].vel_));
    state_interfaces.emplace_back(hardware_interface::StateInterface(
        info_.joints[i].name, hardware_interface::HW_IF_EFFORT, &SerialJointData_[i].tau_));
  }

  // ===== IMU state interfaces =====
  state_interfaces.emplace_back(
    "imu", "orientation.x", &imu_orientation_[0]);
  state_interfaces.emplace_back(
    "imu", "orientation.y", &imu_orientation_[1]);
  state_interfaces.emplace_back(
    "imu", "orientation.z", &imu_orientation_[2]);
  state_interfaces.emplace_back(
    "imu", "orientation.w", &imu_orientation_[3]);

  state_interfaces.emplace_back(
    "imu", "angular_velocity.x", &imu_angular_velocity_[0]);
  state_interfaces.emplace_back(
    "imu", "angular_velocity.y", &imu_angular_velocity_[1]);
  state_interfaces.emplace_back(
    "imu", "angular_velocity.z", &imu_angular_velocity_[2]);

  state_interfaces.emplace_back(
    "imu", "linear_acceleration.x", &imu_linear_acceleration_[0]);
  state_interfaces.emplace_back(
    "imu", "linear_acceleration.y", &imu_linear_acceleration_[1]);
  state_interfaces.emplace_back(
    "imu", "linear_acceleration.z", &imu_linear_acceleration_[2]);

  return state_interfaces;
}

// 导出硬件的命令接口给ros2_control
std::vector<hardware_interface::CommandInterface> LeggedSystemHardware::export_command_interfaces() {
  std::vector<hardware_interface::CommandInterface> command_interfaces;
  // binding command interfaces to SerialJointData_
  for (size_t i = 0; i < info_.joints.size(); i++) {
    command_interfaces.emplace_back(hardware_interface::CommandInterface(
        info_.joints[i].name, hardware_interface::HW_IF_POSITION, &SerialJointData_[i].posDes_));
    command_interfaces.emplace_back(hardware_interface::CommandInterface(
        info_.joints[i].name, hardware_interface::HW_IF_VELOCITY, &SerialJointData_[i].velDes_));
    command_interfaces.emplace_back(hardware_interface::CommandInterface(
        info_.joints[i].name, hardware_interface::HW_IF_EFFORT, &SerialJointData_[i].ff_));
    command_interfaces.emplace_back(
        hardware_interface::CommandInterface(info_.joints[i].name, "Kp", &SerialJointData_[i].kp_));
    command_interfaces.emplace_back(
        hardware_interface::CommandInterface(info_.joints[i].name, "Kd", &SerialJointData_[i].kd_));

    RCLCPP_INFO(rclcpp::get_logger("LeggedSystemHardware"), "%s", command_interfaces[i].get_name().c_str());
  }

  return command_interfaces;
}

hardware_interface::CallbackReturn LeggedSystemHardware::on_activate(
    const rclcpp_lifecycle::State& /*previous_state*/) {
  RCLCPP_INFO(rclcpp::get_logger("LeggedSystemHardware"), "Activating ...please wait...");

  this->node_ = std::make_shared<rclcpp::Node>("hardware_node");

  this->node_->declare_parameter<std::string>("aimrt_cfg_path");
  //this->node_->declare_parameter<double>("state_align_delay_ms", 2.0);
  //this->node_->declare_parameter<int>("state_buffer_capacity", 512);
  auto aimrt_cfg_path = this->node_->get_parameter("aimrt_cfg_path").as_string();
  //const auto state_align_delay_ms = this->node_->get_parameter("state_align_delay_ms").as_double();
  //const auto state_buffer_capacity = this->node_->get_parameter("state_buffer_capacity").as_int();
  RCLCPP_INFO(rclcpp::get_logger("LeggedSystemHardware"), "aimrt_cfg_path: %s", aimrt_cfg_path.c_str());
  //RCLCPP_INFO(rclcpp::get_logger("LeggedSystemHardware"),
  //            "state alignment: delay=%.3f ms, buffer_capacity=%d",
  //            state_align_delay_ms, state_buffer_capacity);
  options_.cfg_file_path = aimrt_cfg_path;
  //state_align_delay_ns_ = static_cast<std::int64_t>(std::llround(state_align_delay_ms * 1e6));
  //state_buffer_capacity_ = std::max<size_t>(32, static_cast<size_t>(state_buffer_capacity));
  
  motorPosPublisher_ = this->node_->create_publisher<std_msgs::msg::Float64MultiArray>("data_analysis/motor_pos", 1);
  motorVelPublisher_ = this->node_->create_publisher<std_msgs::msg::Float64MultiArray>("data_analysis/motor_vel", 1);
  motorTorquePublisher_ =
      this->node_->create_publisher<std_msgs::msg::Float64MultiArray>("data_analysis/motor_torque", 1);
  // readAnkleSpacePublishers removed — X2 uses serial ankle joints, no closed-chain transform
  motorCmdTorquePublisher_ =
      this->node_->create_publisher<std_msgs::msg::Float64MultiArray>("data_analysis/motor_cmd_torque", 1);

  tfBroadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this->node_);

  executor_ = std::make_shared<rclcpp::executors::SingleThreadedExecutor>();
  executor_->add_node(this->node_);
  executor_thread_ = std::thread([this]() { executor_->spin(); });

  aimrtInit();

  return hardware_interface::CallbackReturn::SUCCESS;
}

void LeggedSystemHardware::aimrtInit(){
  // Initialize
  if (!std::filesystem::exists(options_.cfg_file_path)) {
    RCLCPP_ERROR(rclcpp::get_logger("LeggedSystemHardware"),"Config file path does not exist: %s",options_.cfg_file_path.c_str());
    std::exit(EXIT_FAILURE);
  }

  if (!std::filesystem::is_regular_file(options_.cfg_file_path)) {
    RCLCPP_ERROR(rclcpp::get_logger("LeggedSystemHardware"),"Config path exists but is not a regular file: %s",options_.cfg_file_path.c_str());
    std::exit(EXIT_FAILURE);
  }
  
  try {
    RCLCPP_INFO(rclcpp::get_logger("LeggedSystemHardware"), "Initializing AimRTCore, config file: %s", options_.cfg_file_path.c_str());
    core_.Initialize(options_);
    RCLCPP_INFO(rclcpp::get_logger("LeggedSystemHardware"), "AimRTCore initialized successfully");
  } catch (const std::exception& e) {
    RCLCPP_ERROR(rclcpp::get_logger("LeggedSystemHardware"), "AimRTCore initialization error: %s", e.what());
    exit(-1);
  }

  // Create Module
  aimrt::CoreRef module_handle_(core_.GetModuleManager().CreateModule("LeggedSystemModule"));

  // Create Publishers — X2 Ultra topics
  aimrtLegCmdPublisher_ = module_handle_.GetChannelHandle().GetPublisher("/aima/hal/joint/leg/command");
  if (!aimrtLegCmdPublisher_) {
    RCLCPP_ERROR(rclcpp::get_logger("LeggedSystemHardware"), "Get publisher for '/aima/hal/joint/leg/command' failed");
    throw std::runtime_error("Get publisher for '/aima/hal/joint/leg/command' failed");
  }
  aimrtWaistCmdPublisher_ = module_handle_.GetChannelHandle().GetPublisher("/aima/hal/joint/waist/command");
  if (!aimrtWaistCmdPublisher_) {
    RCLCPP_ERROR(rclcpp::get_logger("LeggedSystemHardware"), "Get publisher for '/aima/hal/joint/waist/command' failed");
    throw std::runtime_error("Get publisher for '/aima/hal/joint/waist/command' failed");
  }
  aimrtArmCmdPublisher_ = module_handle_.GetChannelHandle().GetPublisher("/aima/hal/joint/arm/command");
  if (!aimrtArmCmdPublisher_) {
    RCLCPP_ERROR(rclcpp::get_logger("LeggedSystemHardware"), "Get publisher for '/aima/hal/joint/arm/command' failed");
    throw std::runtime_error("Get publisher for '/aima/hal/joint/arm/command' failed");
  }
  aimrtHeadCmdPublisher_ = module_handle_.GetChannelHandle().GetPublisher("/aima/hal/joint/head/command");
  if (!aimrtHeadCmdPublisher_) {
    RCLCPP_ERROR(rclcpp::get_logger("LeggedSystemHardware"), "Get publisher for '/aima/hal/joint/head/command' failed");
    throw std::runtime_error("Get publisher for '/aima/hal/joint/head/command' failed");
  }

  bool ret = aimrt::channel::RegisterPublishType<aimdk_msgs::msg::JointCommandArray>(aimrtLegCmdPublisher_);
  if (!ret) { throw std::runtime_error("RegisterPublishType leg cmd failed"); }

  ret = aimrt::channel::RegisterPublishType<aimdk_msgs::msg::JointCommandArray>(aimrtArmCmdPublisher_);
  if (!ret) { throw std::runtime_error("RegisterPublishType arm cmd failed"); }

  ret = aimrt::channel::RegisterPublishType<aimdk_msgs::msg::JointCommandArray>(aimrtWaistCmdPublisher_);
  if (!ret) { throw std::runtime_error("RegisterPublishType waist cmd failed"); }

  ret = aimrt::channel::RegisterPublishType<aimdk_msgs::msg::JointCommandArray>(aimrtHeadCmdPublisher_);
  if (!ret) { throw std::runtime_error("RegisterPublishType head cmd failed"); }

  aimrtLegCmdPublisherProxy_ =
      std::make_unique<aimrt::channel::PublisherProxy<aimdk_msgs::msg::JointCommandArray>>(aimrtLegCmdPublisher_);
  aimrtArmCmdPublisherProxy_ =
      std::make_unique<aimrt::channel::PublisherProxy<aimdk_msgs::msg::JointCommandArray>>(aimrtArmCmdPublisher_);
  aimrtWaistCmdPublisherProxy_ =
      std::make_unique<aimrt::channel::PublisherProxy<aimdk_msgs::msg::JointCommandArray>>(aimrtWaistCmdPublisher_);
  aimrtHeadCmdPublisherProxy_ =
      std::make_unique<aimrt::channel::PublisherProxy<aimdk_msgs::msg::JointCommandArray>>(aimrtHeadCmdPublisher_);
  
  // Create Subscribers — X2 Ultra topics
  aimrtLegMotorStateSubscriber_ = module_handle_.GetChannelHandle().GetSubscriber("/aima/hal/joint/leg/state");
  if (!aimrtLegMotorStateSubscriber_) {
    throw std::runtime_error("Get subscriber for '/aima/hal/joint/leg/state' failed");
  }
  ret = aimrt::channel::Subscribe<aimdk_msgs::msg::JointStateArray>(
    aimrtLegMotorStateSubscriber_,
    std::bind(&LeggedSystemHardware::legStateCallback, this, std::placeholders::_1));
  if (!ret) { throw std::runtime_error("Subscribe to '/aima/hal/joint/leg/state' failed"); }

  aimrtWaistStateSubscriber_ = module_handle_.GetChannelHandle().GetSubscriber("/aima/hal/joint/waist/state");
  if (!aimrtWaistStateSubscriber_) {
    throw std::runtime_error("Get subscriber for '/aima/hal/joint/waist/state' failed");
  }
  ret = aimrt::channel::Subscribe<aimdk_msgs::msg::JointStateArray>(
    aimrtWaistStateSubscriber_,
    std::bind(&LeggedSystemHardware::waistStateCallback, this, std::placeholders::_1));
  if (!ret) { throw std::runtime_error("Subscribe to '/aima/hal/joint/waist/state' failed"); }

  aimrtArmMotorStateSubscriber_ = module_handle_.GetChannelHandle().GetSubscriber("/aima/hal/joint/arm/state");
  if (!aimrtArmMotorStateSubscriber_) {
    throw std::runtime_error("Get subscriber for '/aima/hal/joint/arm/state' failed");
  }
  ret = aimrt::channel::Subscribe<aimdk_msgs::msg::JointStateArray>(
    aimrtArmMotorStateSubscriber_,
    std::bind(&LeggedSystemHardware::armStateCallback, this, std::placeholders::_1));
  if (!ret) { throw std::runtime_error("Subscribe to '/aima/hal/joint/arm/state' failed"); }

  aimrtHeadStateSubscriber_ = module_handle_.GetChannelHandle().GetSubscriber("/aima/hal/joint/head/state");
  if (!aimrtHeadStateSubscriber_) {
    throw std::runtime_error("Get subscriber for '/aima/hal/joint/head/state' failed");
  }
  ret = aimrt::channel::Subscribe<aimdk_msgs::msg::JointStateArray>(
    aimrtHeadStateSubscriber_,
    std::bind(&LeggedSystemHardware::headStateCallback, this, std::placeholders::_1));
  if (!ret) { throw std::runtime_error("Subscribe to '/aima/hal/joint/head/state' failed"); }

  aimrtImuSubscriber_ = module_handle_.GetChannelHandle().GetSubscriber("/aima/hal/imu/torso/state");
  if (!aimrtImuSubscriber_) {
    throw std::runtime_error("Get subscriber for '/aima/hal/imu/torso/state' failed");
  }
  ret = aimrt::channel::Subscribe<sensor_msgs::msg::Imu>(
    aimrtImuSubscriber_,
    std::bind(&LeggedSystemHardware::imuCallback, this, std::placeholders::_1));
  if (!ret) { throw std::runtime_error("Subscribe to '/aima/hal/imu/torso/state' failed"); }
  
  //start Async
  fu_ = core_.AsyncStart();
}

hardware_interface::CallbackReturn LeggedSystemHardware::on_deactivate(
    const rclcpp_lifecycle::State& /*previous_state*/) {
  RCLCPP_INFO(rclcpp::get_logger("LeggedSystemHardware"), "Deactivating ...please wait...");

  if (executor_thread_.joinable()) {
    executor_->cancel();
    executor_thread_.join();
  }
  
  // Shutdown AimRTCore
  core_.Shutdown();
  fu_.wait();
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::return_type LeggedSystemHardware::read(const rclcpp::Time& /*time*/,
                                                           [[maybe_unused]] const rclcpp::Duration& period) {
  /*
  const auto now_ns = NowSystemNs();
  const auto period_ns = period.nanoseconds();
  if (control_cycle_stamp_ns_ == 0 || period_ns <= 0) {
    control_cycle_stamp_ns_ = now_ns;
  } else {
    control_cycle_stamp_ns_ += period_ns;
    const auto resync_threshold_ns = std::max<std::int64_t>(period_ns * 4, 5'000'000LL);
    if (std::llabs(control_cycle_stamp_ns_ - now_ns) > resync_threshold_ns) {
      control_cycle_stamp_ns_ = now_ns;
    }
  }
  aligned_state_stamp_ns_ = control_cycle_stamp_ns_ - state_align_delay_ns_;

  std::array<MotorDataFloat, kTotalDof> sampled_joint_data{};
  sensor_msgs::msg::Imu sampled_imu;
  {
    std::scoped_lock lock(state_buffer_mutex_);
    std::copy(std::begin(bodyDriveJointData_), std::end(bodyDriveJointData_), sampled_joint_data.begin());
    sampled_imu = bodyDriveIMU_;
    fillAlignedState(aligned_state_stamp_ns_, sampled_joint_data, sampled_imu);
  }
    */

  // Apply direction sign and zero offset (base_motor all-zero for X2)
  for (int i = 0; i < LeggedSystemHardware::kTotalDof; i++) {
    SerialJointData_[i].pos_ = bodyDriveJointData_[i].pos_ * direction_motor[i] + base_motor[i];
    SerialJointData_[i].vel_ = bodyDriveJointData_[i].vel_ * direction_motor[i];
    SerialJointData_[i].tau_ = bodyDriveJointData_[i].tau_ * direction_motor[i];
  }

  for (int i = 0; i < LeggedSystemHardware::kTotalDof; i++) {
    motor_pos_feedback_(i) = SerialJointData_[i].pos_;
    motor_vel_feedback_(i) = SerialJointData_[i].vel_;
    motor_tau_feedback_(i) = SerialJointData_[i].tau_;

    readPosArrayBeforeConversion_[i] = SerialJointData_[i].pos_;
    readVelArrayBeforeConversion_[i] = SerialJointData_[i].vel_;
    readTauArrayBeforeConversion_[i] = SerialJointData_[i].tau_;
  }

  motorPosPublisher_->publish(createFloat64MultiArrayFromVector(motor_pos_feedback_));
  motorVelPublisher_->publish(createFloat64MultiArrayFromVector(motor_vel_feedback_));
  motorTorquePublisher_->publish(createFloat64MultiArrayFromVector(motor_tau_feedback_));

  m_q = Eigen::Map<Eigen::Matrix<double, LeggedSystemHardware::kTotalDof, 1>>(readPosArrayBeforeConversion_);
  m_v = Eigen::Map<Eigen::Matrix<double, LeggedSystemHardware::kTotalDof, 1>>(readVelArrayBeforeConversion_);
  m_t = Eigen::Map<Eigen::Matrix<double, LeggedSystemHardware::kTotalDof, 1>>(readTauArrayBeforeConversion_);

  // X2: serial ankle joints — no closed-chain transform needed
  // BeforeConversion == AfterConversion
  for (int i = 0; i < LeggedSystemHardware::kTotalDof; ++i) {
    readPosArrayAfterConversion_[i] = SerialJointData_[i].pos_;
    readVelArrayAfterConversion_[i] = SerialJointData_[i].vel_;
    readTauArrayAfterConversion_[i] = SerialJointData_[i].tau_;
  }

  // for safety: reset desired commands each cycle
  for (int i = 0; i < LeggedSystemHardware::kTotalDof; ++i) {
    SerialJointData_[i].velDes_ = 0;
    SerialJointData_[i].ff_ = 0;
    SerialJointData_[i].kp_ = 0;
    SerialJointData_[i].kd_ = 1.5;
  }

  imu_orientation_[0] = bodyDriveIMU_.orientation.x;
  imu_orientation_[1] = bodyDriveIMU_.orientation.y;
  imu_orientation_[2] = bodyDriveIMU_.orientation.z;
  imu_orientation_[3] = bodyDriveIMU_.orientation.w;

  imu_angular_velocity_[0] = bodyDriveIMU_.angular_velocity.x;
  imu_angular_velocity_[1] = bodyDriveIMU_.angular_velocity.y;
  imu_angular_velocity_[2] = bodyDriveIMU_.angular_velocity.z;

  imu_linear_acceleration_[0] = bodyDriveIMU_.linear_acceleration.x;
  imu_linear_acceleration_[1] = bodyDriveIMU_.linear_acceleration.y;
  imu_linear_acceleration_[2] = bodyDriveIMU_.linear_acceleration.z;

  return hardware_interface::return_type::OK;
}

hardware_interface::return_type LeggedSystemHardware::write(const rclcpp::Time& /*time*/,
                                                            [[maybe_unused]] const rclcpp::Duration& period) {
  // X2: direct position control, no closed-chain ankle torque mode
  for (int i = 0; i < LeggedSystemHardware::kTotalDof; ++i) {
    writePosDesArray_[i] = SerialJointData_[i].posDes_;
    writeVelDesArray_[i] = SerialJointData_[i].velDes_;
    writeFFArray_[i] = SerialJointData_[i].ff_;
    writeKpArray_[i] = SerialJointData_[i].kp_;
    writeKdArray_[i] = SerialJointData_[i].kd_;
  }

  // Fill leg command (indices 0..11, all serial — no ankle special case)
  //const auto stamp_now = control_cycle_stamp_ns_ > 0 ? control_cycle_stamp_ns_ : NowSystemNs();
  //const auto sequence = command_sequence_++;
  //const auto apply_header = [stamp_now, sequence](auto& msg) {
  //  msg.header.stamp.sec = static_cast<int32_t>(stamp_now / 1000000000LL);
  //  msg.header.stamp.nanosec = static_cast<uint32_t>(stamp_now % 1000000000LL);
  //  msg.header.sequence = sequence;
  //  msg.header.frame_id.clear();
  //};
  //apply_header(legJointCommand_);
  //apply_header(armJointCommand_);
  //apply_header(waistJointCommand_);
  //apply_header(headJointCommand_);

  for (int leg_index = 0; leg_index < 2; leg_index++) { //左,右腿
    for (int i = 0; i < 6; i++) {
      const int idx = i + 6 * leg_index;
      legJointCommand_.joints[idx].position  = writePosDesArray_[idx];
      legJointCommand_.joints[idx].velocity  = writeVelDesArray_[idx];
      legJointCommand_.joints[idx].effort    = writeFFArray_[idx];
      legJointCommand_.joints[idx].stiffness = writeKpArray_[idx];
      legJointCommand_.joints[idx].damping   = writeKdArray_[idx];
    }
  }
  // Fill waist command (indices 12..14 — baseline.yaml order)
  for (int i = 0; i < 3; i++) {
    waistJointCommand_.joints[i].position  = writePosDesArray_[LeggedSystemHardware::kLegDof + i];
    waistJointCommand_.joints[i].velocity  = writeVelDesArray_[LeggedSystemHardware::kLegDof + i];
    waistJointCommand_.joints[i].effort    = writeFFArray_[LeggedSystemHardware::kLegDof + i];
    waistJointCommand_.joints[i].stiffness = writeKpArray_[LeggedSystemHardware::kLegDof + i];
    waistJointCommand_.joints[i].damping   = writeKdArray_[LeggedSystemHardware::kLegDof + i];
  }

  // Fill arm command (indices 15..28 — baseline.yaml order: waist[12..14], arm[15..28])
  for (int arm_index = 0; arm_index < 2; arm_index++) {
    for (int i = 0; i < 7; i++) {
      const int idx = i + 7 * arm_index;
      const int src_idx = LeggedSystemHardware::kLegDof + LeggedSystemHardware::kWaistDof + idx;
      armJointCommand_.joints[idx].position  = writePosDesArray_[src_idx];
      armJointCommand_.joints[idx].velocity  = writeVelDesArray_[src_idx];
      armJointCommand_.joints[idx].effort    = writeFFArray_[src_idx];
      armJointCommand_.joints[idx].stiffness = writeKpArray_[src_idx];
      armJointCommand_.joints[idx].damping   = writeKdArray_[src_idx];
    }
  }

  // Fill head command (indices 29..30)
  for (int i = 0; i < 2; i++) {
    headJointCommand_.joints[i].position  = writePosDesArray_[LeggedSystemHardware::kLegDof + LeggedSystemHardware::kWaistDof + LeggedSystemHardware::kArmDof + i];
    headJointCommand_.joints[i].velocity  = writeVelDesArray_[LeggedSystemHardware::kLegDof + LeggedSystemHardware::kWaistDof + LeggedSystemHardware::kArmDof + i];
    headJointCommand_.joints[i].effort    = writeFFArray_[LeggedSystemHardware::kLegDof + LeggedSystemHardware::kWaistDof + LeggedSystemHardware::kArmDof + i];
    headJointCommand_.joints[i].stiffness = writeKpArray_[LeggedSystemHardware::kLegDof + LeggedSystemHardware::kWaistDof + LeggedSystemHardware::kArmDof + i];
    headJointCommand_.joints[i].damping   = writeKdArray_[LeggedSystemHardware::kLegDof + LeggedSystemHardware::kWaistDof + LeggedSystemHardware::kArmDof + i];
  }

  // Publish all four command topics
  aimrtLegCmdPublisherProxy_->Publish(legJointCommand_);
  aimrtArmCmdPublisherProxy_->Publish(armJointCommand_);
  aimrtWaistCmdPublisherProxy_->Publish(waistJointCommand_);
  aimrtHeadCmdPublisherProxy_->Publish(headJointCommand_);

  if (!firstReceiveArmState || !firstReceiveWaistState || !firstReceiveHeadState) {
    aimrtArmCmdPublisherProxy_->Publish(armJointCommand_);
  }
  // Debug: estimated torque for monitoring
  
  for (int i = 0; i < kTotalDof; ++i) {
    motor_cmd_torque(i) = writeKpArray_[i] * (writePosDesArray_[i] - readPosArrayBeforeConversion_[i]) +
                          writeKdArray_[i] * (writeVelDesArray_[i] - readVelArrayBeforeConversion_[i]);
  }
  motorCmdTorquePublisher_->publish(createFloat64MultiArrayFromVector(motor_cmd_torque));

  return hardware_interface::return_type::OK;
}

}  // namespace legged

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(legged::LeggedSystemHardware, hardware_interface::SystemInterface)
