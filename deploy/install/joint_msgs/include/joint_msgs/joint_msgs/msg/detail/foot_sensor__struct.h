// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from joint_msgs:msg/FootSensor.idl
// generated code does not contain a copyright notice

#ifndef JOINT_MSGS__MSG__DETAIL__FOOT_SENSOR__STRUCT_H_
#define JOINT_MSGS__MSG__DETAIL__FOOT_SENSOR__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'presure'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/FootSensor in the package joint_msgs.
typedef struct joint_msgs__msg__FootSensor
{
  rosidl_runtime_c__int16__Sequence presure;
} joint_msgs__msg__FootSensor;

// Struct for a sequence of joint_msgs__msg__FootSensor.
typedef struct joint_msgs__msg__FootSensor__Sequence
{
  joint_msgs__msg__FootSensor * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} joint_msgs__msg__FootSensor__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // JOINT_MSGS__MSG__DETAIL__FOOT_SENSOR__STRUCT_H_
