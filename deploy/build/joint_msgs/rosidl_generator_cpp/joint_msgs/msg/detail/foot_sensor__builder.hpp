// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from joint_msgs:msg/FootSensor.idl
// generated code does not contain a copyright notice

#ifndef JOINT_MSGS__MSG__DETAIL__FOOT_SENSOR__BUILDER_HPP_
#define JOINT_MSGS__MSG__DETAIL__FOOT_SENSOR__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "joint_msgs/msg/detail/foot_sensor__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace joint_msgs
{

namespace msg
{

namespace builder
{

class Init_FootSensor_presure
{
public:
  Init_FootSensor_presure()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::joint_msgs::msg::FootSensor presure(::joint_msgs::msg::FootSensor::_presure_type arg)
  {
    msg_.presure = std::move(arg);
    return std::move(msg_);
  }

private:
  ::joint_msgs::msg::FootSensor msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::joint_msgs::msg::FootSensor>()
{
  return joint_msgs::msg::builder::Init_FootSensor_presure();
}

}  // namespace joint_msgs

#endif  // JOINT_MSGS__MSG__DETAIL__FOOT_SENSOR__BUILDER_HPP_
