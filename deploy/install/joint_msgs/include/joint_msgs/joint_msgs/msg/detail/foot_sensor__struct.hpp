// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from joint_msgs:msg/FootSensor.idl
// generated code does not contain a copyright notice

#ifndef JOINT_MSGS__MSG__DETAIL__FOOT_SENSOR__STRUCT_HPP_
#define JOINT_MSGS__MSG__DETAIL__FOOT_SENSOR__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__joint_msgs__msg__FootSensor __attribute__((deprecated))
#else
# define DEPRECATED__joint_msgs__msg__FootSensor __declspec(deprecated)
#endif

namespace joint_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct FootSensor_
{
  using Type = FootSensor_<ContainerAllocator>;

  explicit FootSensor_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_init;
  }

  explicit FootSensor_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_init;
    (void)_alloc;
  }

  // field types and members
  using _presure_type =
    std::vector<int16_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<int16_t>>;
  _presure_type presure;

  // setters for named parameter idiom
  Type & set__presure(
    const std::vector<int16_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<int16_t>> & _arg)
  {
    this->presure = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    joint_msgs::msg::FootSensor_<ContainerAllocator> *;
  using ConstRawPtr =
    const joint_msgs::msg::FootSensor_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<joint_msgs::msg::FootSensor_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<joint_msgs::msg::FootSensor_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      joint_msgs::msg::FootSensor_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<joint_msgs::msg::FootSensor_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      joint_msgs::msg::FootSensor_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<joint_msgs::msg::FootSensor_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<joint_msgs::msg::FootSensor_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<joint_msgs::msg::FootSensor_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__joint_msgs__msg__FootSensor
    std::shared_ptr<joint_msgs::msg::FootSensor_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__joint_msgs__msg__FootSensor
    std::shared_ptr<joint_msgs::msg::FootSensor_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const FootSensor_ & other) const
  {
    if (this->presure != other.presure) {
      return false;
    }
    return true;
  }
  bool operator!=(const FootSensor_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct FootSensor_

// alias to use template instance with default allocator
using FootSensor =
  joint_msgs::msg::FootSensor_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace joint_msgs

#endif  // JOINT_MSGS__MSG__DETAIL__FOOT_SENSOR__STRUCT_HPP_
