#pragma once

#include "core.hpp"

#include <filesystem>
#include <future>
#include <stdexcept>
#include <utility>

#include "aimrt_module_cpp_interface/core.h"
#include "aimrt_module_ros2_interface/channel/ros2_channel.h"
#include "core/aimrt_core.h"

#include "aimdk_msgs/msg/joint_command_array.hpp"
#include "aimdk_msgs/msg/joint_state_array.hpp"
#include "sensor_msgs/msg/imu.hpp"
#include "std_msgs/msg/empty.hpp"

namespace aimrl_sdk {

class AimrtTransport final : public Transport {
 public:
  AimrtTransport(const std::string &cfg_file_path) {
    options_.cfg_file_path = cfg_file_path;
  }

  ~AimrtTransport() override {
    try {
      stop();
    } catch (...) {
    }
  }

  void start(Callbacks callbacks) override {
    if (started_) {
      return;
    }
    started_ = true;

    callbacks_ = std::move(callbacks);

    if (!std::filesystem::exists(options_.cfg_file_path)) {
      RCLCPP_ERROR(rclcpp::get_logger("LeggedSystemHardware"), "Config file not found: %s", options_.cfg_file_path.c_str());
      exit(-1);
    }

    aimrt_core_.Initialize(options_);

    // Create Module
    module_handle_ = aimrt::CoreRef(aimrt_core_.GetModuleManager().CreateModule("LeggedSystemModule"));

    // Create Publishers
    aimrtLegCmdPublisher_ = module_handle_.GetChannelHandle().GetPublisher("/aima/hal/joint/leg/command");
    aimrtArmCmdPublisher_ = module_handle_.GetChannelHandle().GetPublisher("/aima/hal/joint/arm/command");
    aimrtWaistCmdPublisher_ = module_handle_.GetChannelHandle().GetPublisher("/aima/hal/joint/waist/command");
    aimrtHeadCmdPublisher_ = module_handle_.GetChannelHandle().GetPublisher("/aima/hal/joint/head/command");
    aimrtSimResetPublisher_ = module_handle_.GetChannelHandle().GetPublisher("/aima/sim/reset");

    bool ret = aimrt::channel::RegisterPublishType<aimdk_msgs::msg::JointCommandArray>(aimrtLegCmdPublisher_);
    if (!ret) {
      throw std::runtime_error("Failed to register publish type for topic '/aima/hal/joint/leg/command'");
    }
    ret = aimrt::channel::RegisterPublishType<aimdk_msgs::msg::JointCommandArray>(aimrtArmCmdPublisher_);
    if (!ret) {
      throw std::runtime_error("Failed to register publish type for topic '/aima/hal/joint/arm/command'");
    }
    ret = aimrt::channel::RegisterPublishType<aimdk_msgs::msg::JointCommandArray>(aimrtWaistCmdPublisher_);
    if (!ret) {
      throw std::runtime_error("Failed to register publish type for topic '/aima/hal/joint/waist/command'");
    }
    ret = aimrt::channel::RegisterPublishType<aimdk_msgs::msg::JointCommandArray>(aimrtHeadCmdPublisher_);
    if (!ret) {
      throw std::runtime_error("Failed to register publish type for topic '/aima/hal/joint/head/command'");
    }
    ret = aimrt::channel::RegisterPublishType<std_msgs::msg::Empty>(aimrtSimResetPublisher_);
    if (!ret) {
      throw std::runtime_error("Failed to register publish type for topic '/aima/sim/reset'");
    }

    aimrtLegCmdPublisherProxy_ = std::make_unique<aimrt::channel::PublisherProxy<aimdk_msgs::msg::JointCommandArray>>(aimrtLegCmdPublisher_);
    aimrtArmCmdPublisherProxy_ = std::make_unique<aimrt::channel::PublisherProxy<aimdk_msgs::msg::JointCommandArray>>(aimrtArmCmdPublisher_);
    aimrtWaistCmdPublisherProxy_ = std::make_unique<aimrt::channel::PublisherProxy<aimdk_msgs::msg::JointCommandArray>>(aimrtWaistCmdPublisher_);
    aimrtHeadCmdPublisherProxy_ = std::make_unique<aimrt::channel::PublisherProxy<aimdk_msgs::msg::JointCommandArray>>(aimrtHeadCmdPublisher_);
    aimrtSimResetPublisherProxy_ = std::make_unique<aimrt::channel::PublisherProxy<std_msgs::msg::Empty>>(aimrtSimResetPublisher_);

    // Create Subscribers
    aimrtArmStateSubscriber_ = module_handle_.GetChannelHandle().GetSubscriber("/aima/hal/joint/arm/state");
    aimrtLegStateSubscriber_ = module_handle_.GetChannelHandle().GetSubscriber("/aima/hal/joint/leg/state");
    aimrtWaistStateSubscriber_ = module_handle_.GetChannelHandle().GetSubscriber("/aima/hal/joint/waist/state");
    aimrtHeadStateSubscriber_ = module_handle_.GetChannelHandle().GetSubscriber("/aima/hal/joint/head/state");
    aimrtImuSubscriber_ = module_handle_.GetChannelHandle().GetSubscriber("/aima/hal/imu/torso/state");


    ret = aimrt::channel::Subscribe<aimdk_msgs::msg::JointStateArray>(aimrtArmStateSubscriber_, std::move(callbacks_.on_arm_state));
    if (!ret) {
      throw std::runtime_error("Failed to subscribe to topic '/aima/hal/joint/arm/state'");
    }
    ret = aimrt::channel::Subscribe<aimdk_msgs::msg::JointStateArray>(aimrtLegStateSubscriber_, std::move(callbacks_.on_leg_state));
    if (!ret) {
      throw std::runtime_error("Failed to subscribe to topic '/aima/hal/joint/leg/state'");
    }
    ret = aimrt::channel::Subscribe<aimdk_msgs::msg::JointStateArray>(aimrtWaistStateSubscriber_, std::move(callbacks_.on_waist_state));
    if (!ret) {
      throw std::runtime_error("Failed to subscribe to topic '/aima/hal/joint/waist/state'");
    }
    ret = aimrt::channel::Subscribe<aimdk_msgs::msg::JointStateArray>(aimrtHeadStateSubscriber_, std::move(callbacks_.on_head_state));
    if (!ret) {
      throw std::runtime_error("Failed to subscribe to topic '/aima/hal/joint/head/state'");
    }
    ret = aimrt::channel::Subscribe<sensor_msgs::msg::Imu>(aimrtImuSubscriber_, std::move(callbacks_.on_imu));
    if (!ret) {
      throw std::runtime_error("Failed to subscribe to topic '/aima/hal/imu/torso/state'");
    }
    shutdown_future_ = aimrt_core_.AsyncStart();
  }

  void stop() override {
    if (!started_) {
      return;
    }
    started_ = false;

    try {
      aimrt_core_.Shutdown();
    } catch (...) {
    }

    // Block until AsyncStart's internal shutdown thread finishes so that
    // AimRTCore's destructor can't run while shutdown is still in progress.
    if (shutdown_future_.valid()) {
      try {
        shutdown_future_.get();
      } catch (...) {
      }
    }
  }

  void publish_arm_command(TimestampNs stamp, Sequence32 seq,
                           const PendingCommand<kArmDof> &cmd,
                           std::span<const std::string> arm_names) override {
    if (!aimrtArmCmdPublisherProxy_ || !cmd.has_any)
      return;
    const auto arm_dof = static_cast<std::size_t>(kArmDof);
    if (arm_names.size() != arm_dof)
      throw std::invalid_argument("arm_names size mismatch");

    aimdk_msgs::msg::JointCommandArray msg;
    const auto stamp_ns = stamp.value;
    msg.header.stamp.sec = static_cast<int32_t>(stamp_ns / 1000000000LL);
    msg.header.stamp.nanosec =
        static_cast<uint32_t>(stamp_ns % 1000000000LL);
    msg.header.sequence = seq.value;

    msg.joints.resize(arm_dof);
    for (std::size_t i = 0; i < arm_dof; ++i) {
      auto &joint = msg.joints[i];
      joint.name = arm_names[i];
      joint.position = cmd.pos[i];
      joint.velocity = cmd.vel[i];
      joint.effort = cmd.eff[i];
      joint.stiffness = cmd.kp[i];
      joint.damping = cmd.kd[i];
    }

    aimrtArmCmdPublisherProxy_->Publish(msg);
  }
  void publish_leg_command(TimestampNs stamp, Sequence32 seq,
                           const PendingCommand<kLegDof> &cmd,
                           std::span<const std::string> leg_names) override {
    if (!aimrtLegCmdPublisherProxy_ || !cmd.has_any)
      return;
    const auto leg_dof = static_cast<std::size_t>(kLegDof);
    if (leg_names.size() != leg_dof)
      throw std::invalid_argument("leg_names size mismatch");

    aimdk_msgs::msg::JointCommandArray msg;
    const auto stamp_ns = stamp.value;
    msg.header.stamp.sec = static_cast<int32_t>(stamp_ns / 1000000000LL);
    msg.header.stamp.nanosec =
        static_cast<uint32_t>(stamp_ns % 1000000000LL);
    msg.header.sequence = seq.value;

    msg.joints.resize(leg_dof);
    for (std::size_t i = 0; i < leg_dof; ++i) {
      auto &joint = msg.joints[i];
      joint.name = leg_names[i];
      joint.position = cmd.pos[i];
      joint.velocity = cmd.vel[i];
      joint.effort = cmd.eff[i];
      joint.stiffness = cmd.kp[i];
      joint.damping = cmd.kd[i];
    }

    aimrtLegCmdPublisherProxy_->Publish(msg);
  }

  void publish_waist_command(TimestampNs stamp, Sequence32 seq,
                           const PendingCommand<kWaistDof> &cmd,
                           std::span<const std::string> waist_names) override {
    if (!aimrtWaistCmdPublisherProxy_ || !cmd.has_any)
      return;
    const auto waist_dof = static_cast<std::size_t>(kWaistDof);
    if (waist_names.size() != waist_dof)
      throw std::invalid_argument("waist_names size mismatch");

    aimdk_msgs::msg::JointCommandArray msg;
    const auto stamp_ns = stamp.value;
    msg.header.stamp.sec = static_cast<int32_t>(stamp_ns / 1000000000LL);
    msg.header.stamp.nanosec =
        static_cast<uint32_t>(stamp_ns % 1000000000LL);
    msg.header.sequence = seq.value;

    msg.joints.resize(waist_dof);
    for (std::size_t i = 0; i < waist_dof; ++i) {
      auto &joint = msg.joints[i];
      joint.name = waist_names[i];
      joint.position = cmd.pos[i];
      joint.velocity = cmd.vel[i];
      joint.effort = cmd.eff[i];
      joint.stiffness = cmd.kp[i];
      joint.damping = cmd.kd[i];
    }

    aimrtWaistCmdPublisherProxy_->Publish(msg);
  }

  void publish_sim_reset() override {
    if (!aimrtSimResetPublisherProxy_)
      return;
    std_msgs::msg::Empty msg;
    aimrtSimResetPublisherProxy_->Publish(msg);
  }

  void publish_head_command(TimestampNs stamp, Sequence32 seq,
                           const PendingCommand<kHeadDof> &cmd,
                           std::span<const std::string> head_names) override {
    if (!aimrtHeadCmdPublisherProxy_ || !cmd.has_any)
      return;
    const auto head_dof = static_cast<std::size_t>(kHeadDof);
    if (head_names.size() != head_dof)
      throw std::invalid_argument("head_names size mismatch");

    aimdk_msgs::msg::JointCommandArray msg;
    const auto stamp_ns = stamp.value;
    msg.header.stamp.sec = static_cast<int32_t>(stamp_ns / 1000000000LL);
    msg.header.stamp.nanosec =
        static_cast<uint32_t>(stamp_ns % 1000000000LL);
    msg.header.sequence = seq.value;

    msg.joints.resize(head_dof);
    for (std::size_t i = 0; i < head_dof; ++i) {
      auto &joint = msg.joints[i];
      joint.name = head_names[i];
      joint.position = cmd.pos[i];
      joint.velocity = cmd.vel[i];
      joint.effort = cmd.eff[i];
      joint.stiffness = cmd.kp[i];
      joint.damping = cmd.kd[i];
    }

    aimrtHeadCmdPublisherProxy_->Publish(msg);
  }

 private:
  bool started_{false};
  std::future<void> shutdown_future_;

  Callbacks callbacks_{};
  aimrt::runtime::core::AimRTCore aimrt_core_;
  aimrt::runtime::core::AimRTCore::Options options_;
  aimrt::CoreRef module_handle_;
  aimrt::channel::SubscriberRef aimrtArmStateSubscriber_;
  aimrt::channel::SubscriberRef aimrtLegStateSubscriber_;
  aimrt::channel::SubscriberRef aimrtWaistStateSubscriber_;
  aimrt::channel::SubscriberRef aimrtHeadStateSubscriber_;
  aimrt::channel::SubscriberRef aimrtImuSubscriber_;
  aimrt::channel::PublisherRef aimrtLegCmdPublisher_;
  aimrt::channel::PublisherRef aimrtArmCmdPublisher_;
  aimrt::channel::PublisherRef aimrtWaistCmdPublisher_;
  aimrt::channel::PublisherRef aimrtHeadCmdPublisher_;
  std::unique_ptr<aimrt::channel::PublisherProxy<aimdk_msgs::msg::JointCommandArray>> aimrtLegCmdPublisherProxy_;
  std::unique_ptr<aimrt::channel::PublisherProxy<aimdk_msgs::msg::JointCommandArray>> aimrtArmCmdPublisherProxy_;
  std::unique_ptr<aimrt::channel::PublisherProxy<aimdk_msgs::msg::JointCommandArray>> aimrtWaistCmdPublisherProxy_;
  std::unique_ptr<aimrt::channel::PublisherProxy<aimdk_msgs::msg::JointCommandArray>> aimrtHeadCmdPublisherProxy_;
  aimrt::channel::PublisherRef aimrtSimResetPublisher_;
  std::unique_ptr<aimrt::channel::PublisherProxy<std_msgs::msg::Empty>> aimrtSimResetPublisherProxy_;
};

}  // namespace aimrl_sdk
