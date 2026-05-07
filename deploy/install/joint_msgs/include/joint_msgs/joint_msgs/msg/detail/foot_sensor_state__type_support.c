// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from joint_msgs:msg/FootSensorState.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "joint_msgs/msg/detail/foot_sensor_state__rosidl_typesupport_introspection_c.h"
#include "joint_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "joint_msgs/msg/detail/foot_sensor_state__functions.h"
#include "joint_msgs/msg/detail/foot_sensor_state__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `names`
#include "rosidl_runtime_c/string_functions.h"
// Member `states`
#include "joint_msgs/msg/foot_sensor.h"
// Member `states`
#include "joint_msgs/msg/detail/foot_sensor__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__FootSensorState_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  joint_msgs__msg__FootSensorState__init(message_memory);
}

void joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__FootSensorState_fini_function(void * message_memory)
{
  joint_msgs__msg__FootSensorState__fini(message_memory);
}

size_t joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__size_function__FootSensorState__names(
  const void * untyped_member)
{
  const rosidl_runtime_c__String__Sequence * member =
    (const rosidl_runtime_c__String__Sequence *)(untyped_member);
  return member->size;
}

const void * joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__get_const_function__FootSensorState__names(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__String__Sequence * member =
    (const rosidl_runtime_c__String__Sequence *)(untyped_member);
  return &member->data[index];
}

void * joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__get_function__FootSensorState__names(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__String__Sequence * member =
    (rosidl_runtime_c__String__Sequence *)(untyped_member);
  return &member->data[index];
}

void joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__fetch_function__FootSensorState__names(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const rosidl_runtime_c__String * item =
    ((const rosidl_runtime_c__String *)
    joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__get_const_function__FootSensorState__names(untyped_member, index));
  rosidl_runtime_c__String * value =
    (rosidl_runtime_c__String *)(untyped_value);
  *value = *item;
}

void joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__assign_function__FootSensorState__names(
  void * untyped_member, size_t index, const void * untyped_value)
{
  rosidl_runtime_c__String * item =
    ((rosidl_runtime_c__String *)
    joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__get_function__FootSensorState__names(untyped_member, index));
  const rosidl_runtime_c__String * value =
    (const rosidl_runtime_c__String *)(untyped_value);
  *item = *value;
}

bool joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__resize_function__FootSensorState__names(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__String__Sequence * member =
    (rosidl_runtime_c__String__Sequence *)(untyped_member);
  rosidl_runtime_c__String__Sequence__fini(member);
  return rosidl_runtime_c__String__Sequence__init(member, size);
}

size_t joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__size_function__FootSensorState__states(
  const void * untyped_member)
{
  const joint_msgs__msg__FootSensor__Sequence * member =
    (const joint_msgs__msg__FootSensor__Sequence *)(untyped_member);
  return member->size;
}

const void * joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__get_const_function__FootSensorState__states(
  const void * untyped_member, size_t index)
{
  const joint_msgs__msg__FootSensor__Sequence * member =
    (const joint_msgs__msg__FootSensor__Sequence *)(untyped_member);
  return &member->data[index];
}

void * joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__get_function__FootSensorState__states(
  void * untyped_member, size_t index)
{
  joint_msgs__msg__FootSensor__Sequence * member =
    (joint_msgs__msg__FootSensor__Sequence *)(untyped_member);
  return &member->data[index];
}

void joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__fetch_function__FootSensorState__states(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const joint_msgs__msg__FootSensor * item =
    ((const joint_msgs__msg__FootSensor *)
    joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__get_const_function__FootSensorState__states(untyped_member, index));
  joint_msgs__msg__FootSensor * value =
    (joint_msgs__msg__FootSensor *)(untyped_value);
  *value = *item;
}

void joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__assign_function__FootSensorState__states(
  void * untyped_member, size_t index, const void * untyped_value)
{
  joint_msgs__msg__FootSensor * item =
    ((joint_msgs__msg__FootSensor *)
    joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__get_function__FootSensorState__states(untyped_member, index));
  const joint_msgs__msg__FootSensor * value =
    (const joint_msgs__msg__FootSensor *)(untyped_value);
  *item = *value;
}

bool joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__resize_function__FootSensorState__states(
  void * untyped_member, size_t size)
{
  joint_msgs__msg__FootSensor__Sequence * member =
    (joint_msgs__msg__FootSensor__Sequence *)(untyped_member);
  joint_msgs__msg__FootSensor__Sequence__fini(member);
  return joint_msgs__msg__FootSensor__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__FootSensorState_message_member_array[3] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(joint_msgs__msg__FootSensorState, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "names",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(joint_msgs__msg__FootSensorState, names),  // bytes offset in struct
    NULL,  // default value
    joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__size_function__FootSensorState__names,  // size() function pointer
    joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__get_const_function__FootSensorState__names,  // get_const(index) function pointer
    joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__get_function__FootSensorState__names,  // get(index) function pointer
    joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__fetch_function__FootSensorState__names,  // fetch(index, &value) function pointer
    joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__assign_function__FootSensorState__names,  // assign(index, value) function pointer
    joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__resize_function__FootSensorState__names  // resize(index) function pointer
  },
  {
    "states",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(joint_msgs__msg__FootSensorState, states),  // bytes offset in struct
    NULL,  // default value
    joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__size_function__FootSensorState__states,  // size() function pointer
    joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__get_const_function__FootSensorState__states,  // get_const(index) function pointer
    joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__get_function__FootSensorState__states,  // get(index) function pointer
    joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__fetch_function__FootSensorState__states,  // fetch(index, &value) function pointer
    joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__assign_function__FootSensorState__states,  // assign(index, value) function pointer
    joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__resize_function__FootSensorState__states  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__FootSensorState_message_members = {
  "joint_msgs__msg",  // message namespace
  "FootSensorState",  // message name
  3,  // number of fields
  sizeof(joint_msgs__msg__FootSensorState),
  joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__FootSensorState_message_member_array,  // message members
  joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__FootSensorState_init_function,  // function to initialize message memory (memory has to be allocated)
  joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__FootSensorState_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__FootSensorState_message_type_support_handle = {
  0,
  &joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__FootSensorState_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_joint_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, joint_msgs, msg, FootSensorState)() {
  joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__FootSensorState_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__FootSensorState_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, joint_msgs, msg, FootSensor)();
  if (!joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__FootSensorState_message_type_support_handle.typesupport_identifier) {
    joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__FootSensorState_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &joint_msgs__msg__FootSensorState__rosidl_typesupport_introspection_c__FootSensorState_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
