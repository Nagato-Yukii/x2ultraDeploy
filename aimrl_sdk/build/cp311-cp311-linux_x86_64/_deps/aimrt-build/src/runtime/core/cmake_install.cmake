# Install script for directory: /home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/tmp/tmp_h9q9_k4/wheel/platlib")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/runtime/core/libaimrt_runtime_core.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aimrt/core" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/aimrt_core.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aimrt/core/allocator" TYPE FILE FILES
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/allocator/allocator_manager.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/allocator/allocator_proxy.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aimrt/core/channel" TYPE FILE FILES
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/channel/channel_backend_base.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/channel/channel_backend_manager.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/channel/channel_backend_tools.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/channel/channel_framework_async_filter.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/channel/channel_handle_proxy.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/channel/channel_manager.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/channel/channel_msg_wrapper.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/channel/channel_registry.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/channel/local_channel_backend.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aimrt/core/configurator" TYPE FILE FILES
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/configurator/configurator_manager.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/configurator/configurator_proxy.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aimrt/core/executor" TYPE FILE FILES
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/executor/asio_strand_executor.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/executor/asio_thread_executor.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/executor/executor_base.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/executor/executor_manager.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/executor/executor_proxy.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/executor/guard_thread_executor.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/executor/main_thread_executor.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/executor/simple_thread_executor.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/executor/tbb_thread_executor.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/executor/time_wheel_executor.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aimrt/core/logger" TYPE FILE FILES
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/logger/console_logger_backend.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/logger/formatter.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/logger/log_data_wrapper.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/logger/log_level_tool.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/logger/logger_backend_base.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/logger/logger_manager.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/logger/logger_proxy.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/logger/os.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/logger/rotate_file_logger_backend.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aimrt/core/module" TYPE FILE FILES
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/module/core_proxy.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/module/module_loader.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/module/module_manager.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aimrt/core/parameter" TYPE FILE FILES
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/parameter/parameter_handle.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/parameter/parameter_handle_proxy.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/parameter/parameter_manager.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aimrt/core/plugin" TYPE FILE FILES
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/plugin/plugin_loader.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/plugin/plugin_manager.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aimrt/core/rpc" TYPE FILE FILES
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/rpc/local_rpc_backend.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/rpc/rpc_backend_base.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/rpc/rpc_backend_manager.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/rpc/rpc_backend_tools.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/rpc/rpc_framework_async_filter.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/rpc/rpc_handle_proxy.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/rpc/rpc_invoke_wrapper.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/rpc/rpc_manager.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/rpc/rpc_registry.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aimrt/core/util" TYPE FILE FILES
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/util/dynamic_lib.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/util/module_detail_info.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/util/rpc_client_tool.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/util/thread_tools.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/util/topic_meta_key.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/util/type_support_pkg_loader.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/util/version.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/runtime/core/util/yaml_tools.h"
    )
endif()

