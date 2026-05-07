// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from joint_msgs:msg/FootSensor.idl
// generated code does not contain a copyright notice
#include "joint_msgs/msg/detail/foot_sensor__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `presure`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
joint_msgs__msg__FootSensor__init(joint_msgs__msg__FootSensor * msg)
{
  if (!msg) {
    return false;
  }
  // presure
  if (!rosidl_runtime_c__int16__Sequence__init(&msg->presure, 0)) {
    joint_msgs__msg__FootSensor__fini(msg);
    return false;
  }
  return true;
}

void
joint_msgs__msg__FootSensor__fini(joint_msgs__msg__FootSensor * msg)
{
  if (!msg) {
    return;
  }
  // presure
  rosidl_runtime_c__int16__Sequence__fini(&msg->presure);
}

bool
joint_msgs__msg__FootSensor__are_equal(const joint_msgs__msg__FootSensor * lhs, const joint_msgs__msg__FootSensor * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // presure
  if (!rosidl_runtime_c__int16__Sequence__are_equal(
      &(lhs->presure), &(rhs->presure)))
  {
    return false;
  }
  return true;
}

bool
joint_msgs__msg__FootSensor__copy(
  const joint_msgs__msg__FootSensor * input,
  joint_msgs__msg__FootSensor * output)
{
  if (!input || !output) {
    return false;
  }
  // presure
  if (!rosidl_runtime_c__int16__Sequence__copy(
      &(input->presure), &(output->presure)))
  {
    return false;
  }
  return true;
}

joint_msgs__msg__FootSensor *
joint_msgs__msg__FootSensor__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  joint_msgs__msg__FootSensor * msg = (joint_msgs__msg__FootSensor *)allocator.allocate(sizeof(joint_msgs__msg__FootSensor), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(joint_msgs__msg__FootSensor));
  bool success = joint_msgs__msg__FootSensor__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
joint_msgs__msg__FootSensor__destroy(joint_msgs__msg__FootSensor * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    joint_msgs__msg__FootSensor__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
joint_msgs__msg__FootSensor__Sequence__init(joint_msgs__msg__FootSensor__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  joint_msgs__msg__FootSensor * data = NULL;

  if (size) {
    data = (joint_msgs__msg__FootSensor *)allocator.zero_allocate(size, sizeof(joint_msgs__msg__FootSensor), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = joint_msgs__msg__FootSensor__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        joint_msgs__msg__FootSensor__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
joint_msgs__msg__FootSensor__Sequence__fini(joint_msgs__msg__FootSensor__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      joint_msgs__msg__FootSensor__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

joint_msgs__msg__FootSensor__Sequence *
joint_msgs__msg__FootSensor__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  joint_msgs__msg__FootSensor__Sequence * array = (joint_msgs__msg__FootSensor__Sequence *)allocator.allocate(sizeof(joint_msgs__msg__FootSensor__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = joint_msgs__msg__FootSensor__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
joint_msgs__msg__FootSensor__Sequence__destroy(joint_msgs__msg__FootSensor__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    joint_msgs__msg__FootSensor__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
joint_msgs__msg__FootSensor__Sequence__are_equal(const joint_msgs__msg__FootSensor__Sequence * lhs, const joint_msgs__msg__FootSensor__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!joint_msgs__msg__FootSensor__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
joint_msgs__msg__FootSensor__Sequence__copy(
  const joint_msgs__msg__FootSensor__Sequence * input,
  joint_msgs__msg__FootSensor__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(joint_msgs__msg__FootSensor);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    joint_msgs__msg__FootSensor * data =
      (joint_msgs__msg__FootSensor *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!joint_msgs__msg__FootSensor__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          joint_msgs__msg__FootSensor__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!joint_msgs__msg__FootSensor__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
