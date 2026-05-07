# Install script for directory: /home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/iceoryx-src/iceoryx_platform

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/iceoryx/v0.1.0" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/generated/iceoryx_platform/include/iceoryx_versions.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/iceoryx_platform" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/iceoryx-src/iceoryx_platform/LICENSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "bin" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/platform/libiceoryx_platform.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/iceoryx/v0.1.0" TYPE DIRECTORY FILES
    "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/iceoryx-src/iceoryx_platform/generic/include/"
    "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/iceoryx-src/iceoryx_platform/linux/include/"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/iceoryx_platform" TYPE FILE FILES
    "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/platform/iceoryx_platformConfigVersion.cmake"
    "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/platform/iceoryx_platformConfig.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/iceoryx_platform/iceoryx_platformTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/iceoryx_platform/iceoryx_platformTargets.cmake"
         "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/platform/CMakeFiles/Export/36eaa3bf62e0ee6534919202822a9994/iceoryx_platformTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/iceoryx_platform/iceoryx_platformTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/iceoryx_platform/iceoryx_platformTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/iceoryx_platform" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/platform/CMakeFiles/Export/36eaa3bf62e0ee6534919202822a9994/iceoryx_platformTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/iceoryx_platform" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/platform/CMakeFiles/Export/36eaa3bf62e0ee6534919202822a9994/iceoryx_platformTargets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/iceoryx/v0.1.0/iceoryx_platform" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/generated/iceoryx_platform/include/iceoryx_platform/platform_settings.hpp")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/iceoryx_platform" TYPE FILE FILES
    "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/iceoryx-src/iceoryx_platform/cmake/IceoryxConfigureOptionMacro.cmake"
    "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/iceoryx-src/iceoryx_platform/cmake/IceoryxPackageHelper.cmake"
    "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/iceoryx-src/iceoryx_platform/cmake/IceoryxPlatform.cmake"
    "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/iceoryx-src/iceoryx_platform/cmake/IceoryxVersion.cmake"
    "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/iceoryx-src/iceoryx_platform/linux/cmake/IceoryxPlatformSettings.cmake"
    )
endif()

