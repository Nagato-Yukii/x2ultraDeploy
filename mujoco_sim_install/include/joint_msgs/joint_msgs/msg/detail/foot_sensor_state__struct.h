// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from joint_msgs:msg/FootSensorState.idl
// generated code does not contain a copyright notice

#ifndef JOINT_MSGS__MSG__DETAIL__FOOT_SENSOR_STATE__STRUCT_H_
#define JOINT_MSGS__MSG__DETAIL__FOOT_SENSOR_STATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'names'
#include "rosidl_runtime_c/string.h"
// Member 'states'
#include "joint_msgs/msg/detail/foot_sensor__struct.h"

/// Struct defined in msg/FootSensorState in the package joint_msgs.
typedef struct joint_msgs__msg__FootSensorState
{
  std_msgs__msg__Header header;
  rosidl_runtime_c__String__Sequence names;
  joint_msgs__msg__FootSensor__Sequence states;
} joint_msgs__msg__FootSensorState;

// Struct for a sequence of joint_msgs__msg__FootSensorState.
typedef struct joint_msgs__msg__FootSensorState__Sequence
{
  joint_msgs__msg__FootSensorState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} joint_msgs__msg__FootSensorState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // JOINT_MSGS__MSG__DETAIL__FOOT_SENSOR_STATE__STRUCT_H_
