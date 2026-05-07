# Install script for directory: /home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk

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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/aimrl_sdk/config" TYPE DIRECTORY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/src/aimrl_sdk/config/")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/_bindings.cpython-311-x86_64-linux-gnu.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/_bindings.cpython-311-x86_64-linux-gnu.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/_bindings.cpython-311-x86_64-linux-gnu.so"
         RPATH "\$ORIGIN")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/aimrl_sdk" TYPE MODULE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_bindings.cpython-311-x86_64-linux-gnu.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/_bindings.cpython-311-x86_64-linux-gnu.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/_bindings.cpython-311-x86_64-linux-gnu.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/_bindings.cpython-311-x86_64-linux-gnu.so"
         OLD_RPATH "/home/suzumiyaharuhi/rl_deploy_v2/lxx2ultra_aimdk/install/aimdk_msgs/lib:/opt/ros/humble/lib:/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/gnu_11.4_cxx20_64_release:"
         NEW_RPATH "\$ORIGIN")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/_bindings.cpython-311-x86_64-linux-gnu.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libaimrt_ros2_plugin.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libaimrt_ros2_plugin.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libaimrt_ros2_plugin.so"
         RPATH "\$ORIGIN")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/aimrl_sdk" TYPE SHARED_LIBRARY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/plugins/ros2_plugin/libaimrt_ros2_plugin.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libaimrt_ros2_plugin.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libaimrt_ros2_plugin.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libaimrt_ros2_plugin.so"
         OLD_RPATH "/opt/ros/humble/lib:/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto:/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/gnu_11.4_cxx20_64_release:"
         NEW_RPATH "\$ORIGIN")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libaimrt_ros2_plugin.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libaimrt_iceoryx_plugin.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libaimrt_iceoryx_plugin.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libaimrt_iceoryx_plugin.so"
         RPATH "\$ORIGIN")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/aimrl_sdk" TYPE SHARED_LIBRARY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/plugins/iceoryx_plugin/libaimrt_iceoryx_plugin.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libaimrt_iceoryx_plugin.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libaimrt_iceoryx_plugin.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libaimrt_iceoryx_plugin.so"
         OLD_RPATH "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/gnu_11.4_cxx20_64_release:"
         NEW_RPATH "\$ORIGIN")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libaimrt_iceoryx_plugin.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_generator_c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_generator_c.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_generator_c.so"
         RPATH "\$ORIGIN")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/aimrl_sdk" TYPE SHARED_LIBRARY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/libros2_plugin_proto__rosidl_generator_c.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_generator_c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_generator_c.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_generator_c.so"
         OLD_RPATH "/opt/ros/humble/lib:"
         NEW_RPATH "\$ORIGIN")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_generator_c.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_c.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_c.so"
         RPATH "\$ORIGIN")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/aimrl_sdk" TYPE SHARED_LIBRARY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/libros2_plugin_proto__rosidl_typesupport_c.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_c.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_c.so"
         OLD_RPATH "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto:/opt/ros/humble/lib:"
         NEW_RPATH "\$ORIGIN")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_c.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_cpp.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_cpp.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_cpp.so"
         RPATH "\$ORIGIN")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/aimrl_sdk" TYPE SHARED_LIBRARY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/libros2_plugin_proto__rosidl_typesupport_cpp.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_cpp.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_cpp.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_cpp.so"
         OLD_RPATH "/opt/ros/humble/lib:"
         NEW_RPATH "\$ORIGIN")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_cpp.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_fastrtps_c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_fastrtps_c.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_fastrtps_c.so"
         RPATH "\$ORIGIN")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/aimrl_sdk" TYPE SHARED_LIBRARY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/libros2_plugin_proto__rosidl_typesupport_fastrtps_c.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_fastrtps_c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_fastrtps_c.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_fastrtps_c.so"
         OLD_RPATH "/opt/ros/humble/lib:/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto:"
         NEW_RPATH "\$ORIGIN")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_fastrtps_c.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_fastrtps_cpp.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_fastrtps_cpp.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_fastrtps_cpp.so"
         RPATH "\$ORIGIN")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/aimrl_sdk" TYPE SHARED_LIBRARY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/libros2_plugin_proto__rosidl_typesupport_fastrtps_cpp.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_fastrtps_cpp.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_fastrtps_cpp.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_fastrtps_cpp.so"
         OLD_RPATH "/opt/ros/humble/lib:"
         NEW_RPATH "\$ORIGIN")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_fastrtps_cpp.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_introspection_c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_introspection_c.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_introspection_c.so"
         RPATH "\$ORIGIN")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/aimrl_sdk" TYPE SHARED_LIBRARY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/libros2_plugin_proto__rosidl_typesupport_introspection_c.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_introspection_c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_introspection_c.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_introspection_c.so"
         OLD_RPATH "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto:/opt/ros/humble/lib:"
         NEW_RPATH "\$ORIGIN")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_introspection_c.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_introspection_cpp.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_introspection_cpp.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_introspection_cpp.so"
         RPATH "\$ORIGIN")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/aimrl_sdk" TYPE SHARED_LIBRARY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/libros2_plugin_proto__rosidl_typesupport_introspection_cpp.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_introspection_cpp.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_introspection_cpp.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_introspection_cpp.so"
         OLD_RPATH "/opt/ros/humble/lib:"
         NEW_RPATH "\$ORIGIN")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/aimrl_sdk/libros2_plugin_proto__rosidl_typesupport_introspection_cpp.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
