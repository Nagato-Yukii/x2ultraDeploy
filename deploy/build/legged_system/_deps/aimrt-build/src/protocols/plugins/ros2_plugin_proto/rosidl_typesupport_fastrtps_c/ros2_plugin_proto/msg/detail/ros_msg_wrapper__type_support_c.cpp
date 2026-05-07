// generated from rosidl_typesupport_fastrtps_c/resource/idl__type_support_c.cpp.em
// with input from ros2_plugin_proto:msg/RosMsgWrapper.idl
// generated code does not contain a copyright notice
#include "ros2_plugin_proto/msg/detail/ros_msg_wrapper__rosidl_typesupport_fastrtps_c.h"


#include <cassert>
#include <limits>
#include <string>
#include "rosidl_typesupport_fastrtps_c/identifier.h"
#include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "ros2_plugin_proto/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "ros2_plugin_proto/msg/detail/ros_msg_wrapper__struct.h"
#include "ros2_plugin_proto/msg/detail/ros_msg_wrapper__functions.h"
#include "fastcdr/Cdr.h"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

// includes and forward declarations of message dependencies and their conversion functions

#if defined(__cplusplus)
extern "C"
{
#endif

#include "rosidl_runtime_c/primitives_sequence.h"  // data
#include "rosidl_runtime_c/primitives_sequence_functions.h"  // data
#include "rosidl_runtime_c/string.h"  // context, serialization_type
#include "rosidl_runtime_c/string_functions.h"  // context, serialization_type

// forward declare type support functions


using _RosMsgWrapper__ros_msg_type = ros2_plugin_proto__msg__RosMsgWrapper;

static bool _RosMsgWrapper__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const _RosMsgWrapper__ros_msg_type * ros_message = static_cast<const _RosMsgWrapper__ros_msg_type *>(untyped_ros_message);
  // Field name: serialization_type
  {
    const rosidl_runtime_c__String * str = &ros_message->serialization_type;
    if (str->capacity == 0 || str->capacity <= str->size) {
      fprintf(stderr, "string capacity not greater than size\n");
      return false;
    }
    if (str->data[str->size] != '\0') {
      fprintf(stderr, "string not null-terminated\n");
      return false;
    }
    cdr << str->data;
  }

  // Field name: context
  {
    size_t size = ros_message->context.size;
    auto array_ptr = ros_message->context.data;
    cdr << static_cast<uint32_t>(size);
    for (size_t i = 0; i < size; ++i) {
      const rosidl_runtime_c__String * str = &array_ptr[i];
      if (str->capacity == 0 || str->capacity <= str->size) {
        fprintf(stderr, "string capacity not greater than size\n");
        return false;
      }
      if (str->data[str->size] != '\0') {
        fprintf(stderr, "string not null-terminated\n");
        return false;
      }
      cdr << str->data;
    }
  }

  // Field name: data
  {
    size_t size = ros_message->data.size;
    auto array_ptr = ros_message->data.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serializeArray(array_ptr, size);
  }

  return true;
}

static bool _RosMsgWrapper__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  _RosMsgWrapper__ros_msg_type * ros_message = static_cast<_RosMsgWrapper__ros_msg_type *>(untyped_ros_message);
  // Field name: serialization_type
  {
    std::string tmp;
    cdr >> tmp;
    if (!ros_message->serialization_type.data) {
      rosidl_runtime_c__String__init(&ros_message->serialization_type);
    }
    bool succeeded = rosidl_runtime_c__String__assign(
      &ros_message->serialization_type,
      tmp.c_str());
    if (!succeeded) {
      fprintf(stderr, "failed to assign string into field 'serialization_type'\n");
      return false;
    }
  }

  // Field name: context
  {
    uint32_t cdrSize;
    cdr >> cdrSize;
    size_t size = static_cast<size_t>(cdrSize);

    // Check there are at least 'size' remaining bytes in the CDR stream before resizing
    auto old_state = cdr.getState();
    bool correct_size = cdr.jump(size);
    cdr.setState(old_state);
    if (!correct_size) {
      fprintf(stderr, "sequence size exceeds remaining buffer\n");
      return false;
    }

    if (ros_message->context.data) {
      rosidl_runtime_c__String__Sequence__fini(&ros_message->context);
    }
    if (!rosidl_runtime_c__String__Sequence__init(&ros_message->context, size)) {
      fprintf(stderr, "failed to create array for field 'context'");
      return false;
    }
    auto array_ptr = ros_message->context.data;
    for (size_t i = 0; i < size; ++i) {
      std::string tmp;
      cdr >> tmp;
      auto & ros_i = array_ptr[i];
      if (!ros_i.data) {
        rosidl_runtime_c__String__init(&ros_i);
      }
      bool succeeded = rosidl_runtime_c__String__assign(
        &ros_i,
        tmp.c_str());
      if (!succeeded) {
        fprintf(stderr, "failed to assign string into field 'context'\n");
        return false;
      }
    }
  }

  // Field name: data
  {
    uint32_t cdrSize;
    cdr >> cdrSize;
    size_t size = static_cast<size_t>(cdrSize);

    // Check there are at least 'size' remaining bytes in the CDR stream before resizing
    auto old_state = cdr.getState();
    bool correct_size = cdr.jump(size);
    cdr.setState(old_state);
    if (!correct_size) {
      fprintf(stderr, "sequence size exceeds remaining buffer\n");
      return false;
    }

    if (ros_message->data.data) {
      rosidl_runtime_c__octet__Sequence__fini(&ros_message->data);
    }
    if (!rosidl_runtime_c__octet__Sequence__init(&ros_message->data, size)) {
      fprintf(stderr, "failed to create array for field 'data'");
      return false;
    }
    auto array_ptr = ros_message->data.data;
    cdr.deserializeArray(array_ptr, size);
  }

  return true;
}  // NOLINT(readability/fn_size)

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ros2_plugin_proto
size_t get_serialized_size_ros2_plugin_proto__msg__RosMsgWrapper(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _RosMsgWrapper__ros_msg_type * ros_message = static_cast<const _RosMsgWrapper__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // field.name serialization_type
  current_alignment += padding +
    eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
    (ros_message->serialization_type.size + 1);
  // field.name context
  {
    size_t array_size = ros_message->context.size;
    auto array_ptr = ros_message->context.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    for (size_t index = 0; index < array_size; ++index) {
      current_alignment += padding +
        eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
        (array_ptr[index].size + 1);
    }
  }
  // field.name data
  {
    size_t array_size = ros_message->data.size;
    auto array_ptr = ros_message->data.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

static uint32_t _RosMsgWrapper__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_ros2_plugin_proto__msg__RosMsgWrapper(
      untyped_ros_message, 0));
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ros2_plugin_proto
size_t max_serialized_size_ros2_plugin_proto__msg__RosMsgWrapper(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;

  // member: serialization_type
  {
    size_t array_size = 1;

    full_bounded = false;
    is_plain = false;
    for (size_t index = 0; index < array_size; ++index) {
      current_alignment += padding +
        eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
        1;
    }
  }
  // member: context
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);

    full_bounded = false;
    is_plain = false;
    for (size_t index = 0; index < array_size; ++index) {
      current_alignment += padding +
        eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
        1;
    }
  }
  // member: data
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);

    last_member_size = array_size * sizeof(uint8_t);
    current_alignment += array_size * sizeof(uint8_t);
  }

  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = ros2_plugin_proto__msg__RosMsgWrapper;
    is_plain =
      (
      offsetof(DataType, data) +
      last_member_size
      ) == ret_val;
  }

  return ret_val;
}

static size_t _RosMsgWrapper__max_serialized_size(char & bounds_info)
{
  bool full_bounded;
  bool is_plain;
  size_t ret_val;

  ret_val = max_serialized_size_ros2_plugin_proto__msg__RosMsgWrapper(
    full_bounded, is_plain, 0);

  bounds_info =
    is_plain ? ROSIDL_TYPESUPPORT_FASTRTPS_PLAIN_TYPE :
    full_bounded ? ROSIDL_TYPESUPPORT_FASTRTPS_BOUNDED_TYPE : ROSIDL_TYPESUPPORT_FASTRTPS_UNBOUNDED_TYPE;
  return ret_val;
}


static message_type_support_callbacks_t __callbacks_RosMsgWrapper = {
  "ros2_plugin_proto::msg",
  "RosMsgWrapper",
  _RosMsgWrapper__cdr_serialize,
  _RosMsgWrapper__cdr_deserialize,
  _RosMsgWrapper__get_serialized_size,
  _RosMsgWrapper__max_serialized_size
};

static rosidl_message_type_support_t _RosMsgWrapper__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_RosMsgWrapper,
  get_message_typesupport_handle_function,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ros2_plugin_proto, msg, RosMsgWrapper)() {
  return &_RosMsgWrapper__type_support;
}

#if defined(__cplusplus)
}
#endif
