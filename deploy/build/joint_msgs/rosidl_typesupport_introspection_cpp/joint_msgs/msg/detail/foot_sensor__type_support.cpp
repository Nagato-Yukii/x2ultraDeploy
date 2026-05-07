// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from joint_msgs:msg/FootSensor.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "joint_msgs/msg/detail/foot_sensor__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace joint_msgs
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void FootSensor_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) joint_msgs::msg::FootSensor(_init);
}

void FootSensor_fini_function(void * message_memory)
{
  auto typed_message = static_cast<joint_msgs::msg::FootSensor *>(message_memory);
  typed_message->~FootSensor();
}

size_t size_function__FootSensor__presure(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<int16_t> *>(untyped_member);
  return member->size();
}

const void * get_const_function__FootSensor__presure(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<int16_t> *>(untyped_member);
  return &member[index];
}

void * get_function__FootSensor__presure(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<int16_t> *>(untyped_member);
  return &member[index];
}

void fetch_function__FootSensor__presure(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const int16_t *>(
    get_const_function__FootSensor__presure(untyped_member, index));
  auto & value = *reinterpret_cast<int16_t *>(untyped_value);
  value = item;
}

void assign_function__FootSensor__presure(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<int16_t *>(
    get_function__FootSensor__presure(untyped_member, index));
  const auto & value = *reinterpret_cast<const int16_t *>(untyped_value);
  item = value;
}

void resize_function__FootSensor__presure(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<int16_t> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember FootSensor_message_member_array[1] = {
  {
    "presure",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(joint_msgs::msg::FootSensor, presure),  // bytes offset in struct
    nullptr,  // default value
    size_function__FootSensor__presure,  // size() function pointer
    get_const_function__FootSensor__presure,  // get_const(index) function pointer
    get_function__FootSensor__presure,  // get(index) function pointer
    fetch_function__FootSensor__presure,  // fetch(index, &value) function pointer
    assign_function__FootSensor__presure,  // assign(index, value) function pointer
    resize_function__FootSensor__presure  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers FootSensor_message_members = {
  "joint_msgs::msg",  // message namespace
  "FootSensor",  // message name
  1,  // number of fields
  sizeof(joint_msgs::msg::FootSensor),
  FootSensor_message_member_array,  // message members
  FootSensor_init_function,  // function to initialize message memory (memory has to be allocated)
  FootSensor_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t FootSensor_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &FootSensor_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace joint_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<joint_msgs::msg::FootSensor>()
{
  return &::joint_msgs::msg::rosidl_typesupport_introspection_cpp::FootSensor_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, joint_msgs, msg, FootSensor)() {
  return &::joint_msgs::msg::rosidl_typesupport_introspection_cpp::FootSensor_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
