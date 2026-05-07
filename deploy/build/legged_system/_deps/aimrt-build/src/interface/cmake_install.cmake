# Install script for directory: /home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/aimrt-src/src/interface

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/suzumiyaharuhi/rl_deploy_v2/deploy/install/legged_system")
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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/aimrt-build/src/interface/aimrt_module_c_interface/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/aimrt-build/src/interface/aimrt_module_cpp_interface/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/aimrt-build/src/interface/aimrt_pkg_c_interface/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/aimrt-build/src/interface/aimrt_type_support_pkg_c_interface/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/aimrt-build/src/interface/aimrt_module_protobuf_interface/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/aimrt-build/src/interface/aimrt_module_ros2_interface/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/aimrt-build/src/interface/aimrt_core_plugin_interface/cmake_install.cmake")
endif()

