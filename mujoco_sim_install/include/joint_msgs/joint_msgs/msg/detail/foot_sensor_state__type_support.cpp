// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from joint_msgs:msg/FootSensorState.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "joint_msgs/msg/detail/foot_sensor_state__struct.hpp"
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

void FootSensorState_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) joint_msgs::msg::FootSensorState(_init);
}

void FootSensorState_fini_function(void * message_memory)
{
  auto typed_message = static_cast<joint_msgs::msg::FootSensorState *>(message_memory);
  typed_message->~FootSensorState();
}

size_t size_function__FootSensorState__names(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<std::string> *>(untyped_member);
  return member->size();
}

const void * get_const_function__FootSensorState__names(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<std::string> *>(untyped_member);
  return &member[index];
}

void * get_function__FootSensorState__names(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<std::string> *>(untyped_member);
  return &member[index];
}

void fetch_function__FootSensorState__names(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const std::string *>(
    get_const_function__FootSensorState__names(untyped_member, index));
  auto & value = *reinterpret_cast<std::string *>(untyped_value);
  value = item;
}

void assign_function__FootSensorState__names(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<std::string *>(
    get_function__FootSensorState__names(untyped_member, index));
  const auto & value = *reinterpret_cast<const std::string *>(untyped_value);
  item = value;
}

void resize_function__FootSensorState__names(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<std::string> *>(untyped_member);
  member->resize(size);
}

size_t size_function__FootSensorState__states(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<joint_msgs::msg::FootSensor> *>(untyped_member);
  return member->size();
}

const void * get_const_function__FootSensorState__states(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<joint_msgs::msg::FootSensor> *>(untyped_member);
  return &member[index];
}

void * get_function__FootSensorState__states(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<joint_msgs::msg::FootSensor> *>(untyped_member);
  return &member[index];
}

void fetch_function__FootSensorState__states(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const joint_msgs::msg::FootSensor *>(
    get_const_function__FootSensorState__states(untyped_member, index));
  auto & value = *reinterpret_cast<joint_msgs::msg::FootSensor *>(untyped_value);
  value = item;
}

void assign_function__FootSensorState__states(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<joint_msgs::msg::FootSensor *>(
    get_function__FootSensorState__states(untyped_member, index));
  const auto & value = *reinterpret_cast<const joint_msgs::msg::FootSensor *>(untyped_value);
  item = value;
}

void resize_function__FootSensorState__states(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<joint_msgs::msg::FootSensor> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember FootSensorState_message_member_array[3] = {
  {
    "header",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<std_msgs::msg::Header>(),  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(joint_msgs::msg::FootSensorState, header),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "names",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(joint_msgs::msg::FootSensorState, names),  // bytes offset in struct
    nullptr,  // default value
    size_function__FootSensorState__names,  // size() function pointer
    get_const_function__FootSensorState__names,  // get_const(index) function pointer
    get_function__FootSensorState__names,  // get(index) function pointer
    fetch_function__FootSensorState__names,  // fetch(index, &value) function pointer
    assign_function__FootSensorState__names,  // assign(index, value) function pointer
    resize_function__FootSensorState__names  // resize(index) function pointer
  },
  {
    "states",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<joint_msgs::msg::FootSensor>(),  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(joint_msgs::msg::FootSensorState, states),  // bytes offset in struct
    nullptr,  // default value
    size_function__FootSensorState__states,  // size() function pointer
    get_const_function__FootSensorState__states,  // get_const(index) function pointer
    get_function__FootSensorState__states,  // get(index) function pointer
    fetch_function__FootSensorState__states,  // fetch(index, &value) function pointer
    assign_function__FootSensorState__states,  // assign(index, value) function pointer
    resize_function__FootSensorState__states  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers FootSensorState_message_members = {
  "joint_msgs::msg",  // message namespace
  "FootSensorState",  // message name
  3,  // number of fields
  sizeof(joint_msgs::msg::FootSensorState),
  FootSensorState_message_member_array,  // message members
  FootSensorState_init_function,  // function to initialize message memory (memory has to be allocated)
  FootSensorState_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t FootSensorState_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &FootSensorState_message_members,
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
get_message_type_support_handle<joint_msgs::msg::FootSensorState>()
{
  return &::joint_msgs::msg::rosidl_typesupport_introspection_cpp::FootSensorState_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, joint_msgs, msg, FootSensorState)() {
  return &::joint_msgs::msg::rosidl_typesupport_introspection_cpp::FootSensorState_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
