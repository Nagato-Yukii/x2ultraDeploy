// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__type_support.cpp.em
// with input from ros2_plugin_proto:msg/RosMsgWrapper.idl
// generated code does not contain a copyright notice
#include "ros2_plugin_proto/msg/detail/ros_msg_wrapper__rosidl_typesupport_fastrtps_cpp.hpp"
#include "ros2_plugin_proto/msg/detail/ros_msg_wrapper__struct.hpp"

#include <limits>
#include <stdexcept>
#include <string>
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_fastrtps_cpp/identifier.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_fastrtps_cpp/wstring_conversion.hpp"
#include "fastcdr/Cdr.h"


// forward declaration of message dependencies and their conversion functions

namespace ros2_plugin_proto
{

namespace msg
{

namespace typesupport_fastrtps_cpp
{

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_ros2_plugin_proto
cdr_serialize(
  const ros2_plugin_proto::msg::RosMsgWrapper & ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Member: serialization_type
  cdr << ros_message.serialization_type;
  // Member: context
  {
    cdr << ros_message.context;
  }
  // Member: data
  {
    cdr << ros_message.data;
  }
  return true;
}

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_ros2_plugin_proto
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  ros2_plugin_proto::msg::RosMsgWrapper & ros_message)
{
  // Member: serialization_type
  cdr >> ros_message.serialization_type;

  // Member: context
  {
    cdr >> ros_message.context;
  }

  // Member: data
  {
    cdr >> ros_message.data;
  }

  return true;
}  // NOLINT(readability/fn_size)

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_ros2_plugin_proto
get_serialized_size(
  const ros2_plugin_proto::msg::RosMsgWrapper & ros_message,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Member: serialization_type
  current_alignment += padding +
    eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
    (ros_message.serialization_type.size() + 1);
  // Member: context
  {
    size_t array_size = ros_message.context.size();

    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    for (size_t index = 0; index < array_size; ++index) {
      current_alignment += padding +
        eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
        (ros_message.context[index].size() + 1);
    }
  }
  // Member: data
  {
    size_t array_size = ros_message.data.size();

    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    size_t item_size = sizeof(ros_message.data[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_ros2_plugin_proto
max_serialized_size_RosMsgWrapper(
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


  // Member: serialization_type
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

  // Member: context
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

  // Member: data
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
    using DataType = ros2_plugin_proto::msg::RosMsgWrapper;
    is_plain =
      (
      offsetof(DataType, data) +
      last_member_size
      ) == ret_val;
  }

  return ret_val;
}

static bool _RosMsgWrapper__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  auto typed_message =
    static_cast<const ros2_plugin_proto::msg::RosMsgWrapper *>(
    untyped_ros_message);
  return cdr_serialize(*typed_message, cdr);
}

static bool _RosMsgWrapper__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  auto typed_message =
    static_cast<ros2_plugin_proto::msg::RosMsgWrapper *>(
    untyped_ros_message);
  return cdr_deserialize(cdr, *typed_message);
}

static uint32_t _RosMsgWrapper__get_serialized_size(
  const void * untyped_ros_message)
{
  auto typed_message =
    static_cast<const ros2_plugin_proto::msg::RosMsgWrapper *>(
    untyped_ros_message);
  return static_cast<uint32_t>(get_serialized_size(*typed_message, 0));
}

static size_t _RosMsgWrapper__max_serialized_size(char & bounds_info)
{
  bool full_bounded;
  bool is_plain;
  size_t ret_val;

  ret_val = max_serialized_size_RosMsgWrapper(full_bounded, is_plain, 0);

  bounds_info =
    is_plain ? ROSIDL_TYPESUPPORT_FASTRTPS_PLAIN_TYPE :
    full_bounded ? ROSIDL_TYPESUPPORT_FASTRTPS_BOUNDED_TYPE : ROSIDL_TYPESUPPORT_FASTRTPS_UNBOUNDED_TYPE;
  return ret_val;
}

static message_type_support_callbacks_t _RosMsgWrapper__callbacks = {
  "ros2_plugin_proto::msg",
  "RosMsgWrapper",
  _RosMsgWrapper__cdr_serialize,
  _RosMsgWrapper__cdr_deserialize,
  _RosMsgWrapper__get_serialized_size,
  _RosMsgWrapper__max_serialized_size
};

static rosidl_message_type_support_t _RosMsgWrapper__handle = {
  rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
  &_RosMsgWrapper__callbacks,
  get_message_typesupport_handle_function,
};

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace ros2_plugin_proto

namespace rosidl_typesupport_fastrtps_cpp
{

template<>
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_EXPORT_ros2_plugin_proto
const rosidl_message_type_support_t *
get_message_type_support_handle<ros2_plugin_proto::msg::RosMsgWrapper>()
{
  return &ros2_plugin_proto::msg::typesupport_fastrtps_cpp::_RosMsgWrapper__handle;
}

}  // namespace rosidl_typesupport_fastrtps_cpp

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, ros2_plugin_proto, msg, RosMsgWrapper)() {
  return &ros2_plugin_proto::msg::typesupport_fastrtps_cpp::_RosMsgWrapper__handle;
}

#ifdef __cplusplus
}
#endif
