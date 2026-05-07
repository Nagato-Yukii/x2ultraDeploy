// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from joint_msgs:msg/FootSensorState.idl
// generated code does not contain a copyright notice

#ifndef JOINT_MSGS__MSG__DETAIL__FOOT_SENSOR_STATE__STRUCT_HPP_
#define JOINT_MSGS__MSG__DETAIL__FOOT_SENSOR_STATE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"
// Member 'states'
#include "joint_msgs/msg/detail/foot_sensor__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__joint_msgs__msg__FootSensorState __attribute__((deprecated))
#else
# define DEPRECATED__joint_msgs__msg__FootSensorState __declspec(deprecated)
#endif

namespace joint_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct FootSensorState_
{
  using Type = FootSensorState_<ContainerAllocator>;

  explicit FootSensorState_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    (void)_init;
  }

  explicit FootSensorState_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _names_type =
    std::vector<std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>>>;
  _names_type names;
  using _states_type =
    std::vector<joint_msgs::msg::FootSensor_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<joint_msgs::msg::FootSensor_<ContainerAllocator>>>;
  _states_type states;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__names(
    const std::vector<std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>>> & _arg)
  {
    this->names = _arg;
    return *this;
  }
  Type & set__states(
    const std::vector<joint_msgs::msg::FootSensor_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<joint_msgs::msg::FootSensor_<ContainerAllocator>>> & _arg)
  {
    this->states = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    joint_msgs::msg::FootSensorState_<ContainerAllocator> *;
  using ConstRawPtr =
    const joint_msgs::msg::FootSensorState_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<joint_msgs::msg::FootSensorState_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<joint_msgs::msg::FootSensorState_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      joint_msgs::msg::FootSensorState_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<joint_msgs::msg::FootSensorState_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      joint_msgs::msg::FootSensorState_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<joint_msgs::msg::FootSensorState_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<joint_msgs::msg::FootSensorState_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<joint_msgs::msg::FootSensorState_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__joint_msgs__msg__FootSensorState
    std::shared_ptr<joint_msgs::msg::FootSensorState_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__joint_msgs__msg__FootSensorState
    std::shared_ptr<joint_msgs::msg::FootSensorState_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const FootSensorState_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->names != other.names) {
      return false;
    }
    if (this->states != other.states) {
      return false;
    }
    return true;
  }
  bool operator!=(const FootSensorState_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct FootSensorState_

// alias to use template instance with default allocator
using FootSensorState =
  joint_msgs::msg::FootSensorState_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace joint_msgs

#endif  // JOINT_MSGS__MSG__DETAIL__FOOT_SENSOR_STATE__STRUCT_HPP_
