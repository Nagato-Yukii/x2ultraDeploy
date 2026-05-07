// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from joint_msgs:msg/FootSensor.idl
// generated code does not contain a copyright notice

#ifndef JOINT_MSGS__MSG__DETAIL__FOOT_SENSOR__TRAITS_HPP_
#define JOINT_MSGS__MSG__DETAIL__FOOT_SENSOR__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "joint_msgs/msg/detail/foot_sensor__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace joint_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const FootSensor & msg,
  std::ostream & out)
{
  out << "{";
  // member: presure
  {
    if (msg.presure.size() == 0) {
      out << "presure: []";
    } else {
      out << "presure: [";
      size_t pending_items = msg.presure.size();
      for (auto item : msg.presure) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const FootSensor & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: presure
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.presure.size() == 0) {
      out << "presure: []\n";
    } else {
      out << "presure:\n";
      for (auto item : msg.presure) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const FootSensor & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace joint_msgs

namespace rosidl_generator_traits
{

[[deprecated("use joint_msgs::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const joint_msgs::msg::FootSensor & msg,
  std::ostream & out, size_t indentation = 0)
{
  joint_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use joint_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const joint_msgs::msg::FootSensor & msg)
{
  return joint_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<joint_msgs::msg::FootSensor>()
{
  return "joint_msgs::msg::FootSensor";
}

template<>
inline const char * name<joint_msgs::msg::FootSensor>()
{
  return "joint_msgs/msg/FootSensor";
}

template<>
struct has_fixed_size<joint_msgs::msg::FootSensor>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<joint_msgs::msg::FootSensor>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<joint_msgs::msg::FootSensor>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // JOINT_MSGS__MSG__DETAIL__FOOT_SENSOR__TRAITS_HPP_
