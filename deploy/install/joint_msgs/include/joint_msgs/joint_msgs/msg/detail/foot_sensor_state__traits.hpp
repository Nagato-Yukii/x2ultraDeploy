// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from joint_msgs:msg/FootSensorState.idl
// generated code does not contain a copyright notice

#ifndef JOINT_MSGS__MSG__DETAIL__FOOT_SENSOR_STATE__TRAITS_HPP_
#define JOINT_MSGS__MSG__DETAIL__FOOT_SENSOR_STATE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "joint_msgs/msg/detail/foot_sensor_state__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"
// Member 'states'
#include "joint_msgs/msg/detail/foot_sensor__traits.hpp"

namespace joint_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const FootSensorState & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: names
  {
    if (msg.names.size() == 0) {
      out << "names: []";
    } else {
      out << "names: [";
      size_t pending_items = msg.names.size();
      for (auto item : msg.names) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: states
  {
    if (msg.states.size() == 0) {
      out << "states: []";
    } else {
      out << "states: [";
      size_t pending_items = msg.states.size();
      for (auto item : msg.states) {
        to_flow_style_yaml(item, out);
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
  const FootSensorState & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: header
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "header:\n";
    to_block_style_yaml(msg.header, out, indentation + 2);
  }

  // member: names
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.names.size() == 0) {
      out << "names: []\n";
    } else {
      out << "names:\n";
      for (auto item : msg.names) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: states
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.states.size() == 0) {
      out << "states: []\n";
    } else {
      out << "states:\n";
      for (auto item : msg.states) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const FootSensorState & msg, bool use_flow_style = false)
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
  const joint_msgs::msg::FootSensorState & msg,
  std::ostream & out, size_t indentation = 0)
{
  joint_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use joint_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const joint_msgs::msg::FootSensorState & msg)
{
  return joint_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<joint_msgs::msg::FootSensorState>()
{
  return "joint_msgs::msg::FootSensorState";
}

template<>
inline const char * name<joint_msgs::msg::FootSensorState>()
{
  return "joint_msgs/msg/FootSensorState";
}

template<>
struct has_fixed_size<joint_msgs::msg::FootSensorState>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<joint_msgs::msg::FootSensorState>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<joint_msgs::msg::FootSensorState>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // JOINT_MSGS__MSG__DETAIL__FOOT_SENSOR_STATE__TRAITS_HPP_
