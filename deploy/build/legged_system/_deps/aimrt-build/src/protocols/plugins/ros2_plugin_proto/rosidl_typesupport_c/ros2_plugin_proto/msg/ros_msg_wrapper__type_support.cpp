// generated from rosidl_typesupport_c/resource/idl__type_support.cpp.em
// with input from ros2_plugin_proto:msg/RosMsgWrapper.idl
// generated code does not contain a copyright notice

#include "cstddef"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "ros2_plugin_proto/msg/detail/ros_msg_wrapper__struct.h"
#include "ros2_plugin_proto/msg/detail/ros_msg_wrapper__type_support.h"
#include "rosidl_typesupport_c/identifier.h"
#include "rosidl_typesupport_c/message_type_support_dispatch.h"
#include "rosidl_typesupport_c/type_support_map.h"
#include "rosidl_typesupport_c/visibility_control.h"
#include "rosidl_typesupport_interface/macros.h"

namespace ros2_plugin_proto
{

namespace msg
{

namespace rosidl_typesupport_c
{

typedef struct _RosMsgWrapper_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _RosMsgWrapper_type_support_ids_t;

static const _RosMsgWrapper_type_support_ids_t _RosMsgWrapper_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_c",  // ::rosidl_typesupport_fastrtps_c::typesupport_identifier,
    "rosidl_typesupport_introspection_c",  // ::rosidl_typesupport_introspection_c::typesupport_identifier,
  }
};

typedef struct _RosMsgWrapper_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _RosMsgWrapper_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _RosMsgWrapper_type_support_symbol_names_t _RosMsgWrapper_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ros2_plugin_proto, msg, RosMsgWrapper)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_plugin_proto, msg, RosMsgWrapper)),
  }
};

typedef struct _RosMsgWrapper_type_support_data_t
{
  void * data[2];
} _RosMsgWrapper_type_support_data_t;

static _RosMsgWrapper_type_support_data_t _RosMsgWrapper_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _RosMsgWrapper_message_typesupport_map = {
  2,
  "ros2_plugin_proto",
  &_RosMsgWrapper_message_typesupport_ids.typesupport_identifier[0],
  &_RosMsgWrapper_message_typesupport_symbol_names.symbol_name[0],
  &_RosMsgWrapper_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t RosMsgWrapper_message_type_support_handle = {
  rosidl_typesupport_c__typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_RosMsgWrapper_message_typesupport_map),
  rosidl_typesupport_c__get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_c

}  // namespace msg

}  // namespace ros2_plugin_proto

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_c, ros2_plugin_proto, msg, RosMsgWrapper)() {
  return &::ros2_plugin_proto::msg::rosidl_typesupport_c::RosMsgWrapper_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
