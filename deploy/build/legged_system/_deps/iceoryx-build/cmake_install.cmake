# Install script for directory: /home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/iceoryx-src/iceoryx_meta

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

if(CMAKE_INSTALL_COMPONENT STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/iceoryx" TYPE FILE FILES
    "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/iceoryx-src/iceoryx_meta/../doc/website/FAQ.md"
    "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/iceoryx-src/iceoryx_meta/../doc/website/getting-started/overview.md"
    "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/iceoryx-src/iceoryx_meta/../doc/website/advanced/installation-guide-for-contributors.md"
    "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/iceoryx-src/iceoryx_meta/../LICENSE"
    "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/iceoryx-src/iceoryx_meta/../NOTICE.md"
    "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/iceoryx-src/iceoryx_meta/../README.md"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/iceoryx" TYPE DIRECTORY FILES "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/iceoryx-src/iceoryx_meta/../doc/3rd_party_licenses")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/iceoryx" TYPE DIRECTORY FILES "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/iceoryx-src/iceoryx_meta/../doc/website/release-notes")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/platform/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/hoofs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/posh/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/iceoryx_introspection/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/iceoryx_binding_c/cmake_install.cmake")
endif()

