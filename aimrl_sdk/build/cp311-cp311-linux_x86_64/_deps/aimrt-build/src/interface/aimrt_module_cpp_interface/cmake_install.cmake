# Install script for directory: /home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aimrt/aimrt_module_cpp_interface" TYPE FILE FILES
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/aimrt_module_cpp_interface.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/core.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/module_base.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aimrt/aimrt_module_cpp_interface/allocator" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/allocator/allocator.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aimrt/aimrt_module_cpp_interface/channel" TYPE FILE FILES
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/channel/channel_context.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/channel/channel_handle.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aimrt/aimrt_module_cpp_interface/co" TYPE FILE FILES
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/co/aimrt_context.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/co/async_scope.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/co/inline_scheduler.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/co/on.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/co/schedule.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/co/start_detached.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/co/sync_wait.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/co/task.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/co/then.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aimrt/aimrt_module_cpp_interface/configurator" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/configurator/configurator.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aimrt/aimrt_module_cpp_interface/executor" TYPE FILE FILES
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/executor/executor.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/executor/executor_manager.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/executor/timer.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aimrt/aimrt_module_cpp_interface/logger" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/logger/logger.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aimrt/aimrt_module_cpp_interface/parameter" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/parameter/parameter_handle.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aimrt/aimrt_module_cpp_interface/rpc" TYPE FILE FILES
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/rpc/rpc_co_filter.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/rpc/rpc_context.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/rpc/rpc_handle.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/rpc/rpc_status.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aimrt/aimrt_module_cpp_interface/util" TYPE FILE FILES
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/util/buffer.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/util/buffer_array_allocator.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/util/function.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/util/string.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/util/type_support.h"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/interface/aimrt_module_cpp_interface/util/version.h"
    )
endif()

