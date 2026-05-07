// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from joint_msgs:msg/FootSensorState.idl
// generated code does not contain a copyright notice

#ifndef JOINT_MSGS__MSG__DETAIL__FOOT_SENSOR_STATE__BUILDER_HPP_
#define JOINT_MSGS__MSG__DETAIL__FOOT_SENSOR_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "joint_msgs/msg/detail/foot_sensor_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace joint_msgs
{

namespace msg
{

namespace builder
{

class Init_FootSensorState_states
{
public:
  explicit Init_FootSensorState_states(::joint_msgs::msg::FootSensorState & msg)
  : msg_(msg)
  {}
  ::joint_msgs::msg::FootSensorState states(::joint_msgs::msg::FootSensorState::_states_type arg)
  {
    msg_.states = std::move(arg);
    return std::move(msg_);
  }

private:
  ::joint_msgs::msg::FootSensorState msg_;
};

class Init_FootSensorState_names
{
public:
  explicit Init_FootSensorState_names(::joint_msgs::msg::FootSensorState & msg)
  : msg_(msg)
  {}
  Init_FootSensorState_states names(::joint_msgs::msg::FootSensorState::_names_type arg)
  {
    msg_.names = std::move(arg);
    return Init_FootSensorState_states(msg_);
  }

private:
  ::joint_msgs::msg::FootSensorState msg_;
};

class Init_FootSensorState_header
{
public:
  Init_FootSensorState_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_FootSensorState_names header(::joint_msgs::msg::FootSensorState::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_FootSensorState_names(msg_);
  }

private:
  ::joint_msgs::msg::FootSensorState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::joint_msgs::msg::FootSensorState>()
{
  return joint_msgs::msg::builder::Init_FootSensorState_header();
}

}  // namespace joint_msgs

#endif  // JOINT_MSGS__MSG__DETAIL__FOOT_SENSOR_STATE__BUILDER_HPP_
