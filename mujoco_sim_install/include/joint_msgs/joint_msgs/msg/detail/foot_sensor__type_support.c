// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from joint_msgs:msg/FootSensor.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "joint_msgs/msg/detail/foot_sensor__rosidl_typesupport_introspection_c.h"
#include "joint_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "joint_msgs/msg/detail/foot_sensor__functions.h"
#include "joint_msgs/msg/detail/foot_sensor__struct.h"


// Include directives for member types
// Member `presure`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__FootSensor_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  joint_msgs__msg__FootSensor__init(message_memory);
}

void joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__FootSensor_fini_function(void * message_memory)
{
  joint_msgs__msg__FootSensor__fini(message_memory);
}

size_t joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__size_function__FootSensor__presure(
  const void * untyped_member)
{
  const rosidl_runtime_c__int16__Sequence * member =
    (const rosidl_runtime_c__int16__Sequence *)(untyped_member);
  return member->size;
}

const void * joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__get_const_function__FootSensor__presure(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__int16__Sequence * member =
    (const rosidl_runtime_c__int16__Sequence *)(untyped_member);
  return &member->data[index];
}

void * joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__get_function__FootSensor__presure(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__int16__Sequence * member =
    (rosidl_runtime_c__int16__Sequence *)(untyped_member);
  return &member->data[index];
}

void joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__fetch_function__FootSensor__presure(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const int16_t * item =
    ((const int16_t *)
    joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__get_const_function__FootSensor__presure(untyped_member, index));
  int16_t * value =
    (int16_t *)(untyped_value);
  *value = *item;
}

void joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__assign_function__FootSensor__presure(
  void * untyped_member, size_t index, const void * untyped_value)
{
  int16_t * item =
    ((int16_t *)
    joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__get_function__FootSensor__presure(untyped_member, index));
  const int16_t * value =
    (const int16_t *)(untyped_value);
  *item = *value;
}

bool joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__resize_function__FootSensor__presure(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__int16__Sequence * member =
    (rosidl_runtime_c__int16__Sequence *)(untyped_member);
  rosidl_runtime_c__int16__Sequence__fini(member);
  return rosidl_runtime_c__int16__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__FootSensor_message_member_array[1] = {
  {
    "presure",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(joint_msgs__msg__FootSensor, presure),  // bytes offset in struct
    NULL,  // default value
    joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__size_function__FootSensor__presure,  // size() function pointer
    joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__get_const_function__FootSensor__presure,  // get_const(index) function pointer
    joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__get_function__FootSensor__presure,  // get(index) function pointer
    joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__fetch_function__FootSensor__presure,  // fetch(index, &value) function pointer
    joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__assign_function__FootSensor__presure,  // assign(index, value) function pointer
    joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__resize_function__FootSensor__presure  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__FootSensor_message_members = {
  "joint_msgs__msg",  // message namespace
  "FootSensor",  // message name
  1,  // number of fields
  sizeof(joint_msgs__msg__FootSensor),
  joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__FootSensor_message_member_array,  // message members
  joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__FootSensor_init_function,  // function to initialize message memory (memory has to be allocated)
  joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__FootSensor_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__FootSensor_message_type_support_handle = {
  0,
  &joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__FootSensor_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_joint_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, joint_msgs, msg, FootSensor)() {
  if (!joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__FootSensor_message_type_support_handle.typesupport_identifier) {
    joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__FootSensor_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &joint_msgs__msg__FootSensor__rosidl_typesupport_introspection_c__FootSensor_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
