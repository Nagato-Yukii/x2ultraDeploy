# Install script for directory: /home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/protocols/plugins/ros2_plugin_proto

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ament_index/resource_index/rosidl_interfaces" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/ament_cmake_index/share/ament_index/resource_index/rosidl_interfaces/ros2_plugin_proto")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ros2_plugin_proto/ros2_plugin_proto" TYPE DIRECTORY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/rosidl_generator_c/ros2_plugin_proto/" REGEX "/[^/]*\\.h$")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/environment" TYPE FILE FILES "/opt/ros/humble/lib/python3.10/site-packages/ament_package/template/environment_hook/library_path.sh")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/environment" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/ament_cmake_environment_hooks/library_path.dsv")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_generator_c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_generator_c.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_generator_c.so"
         RPATH "\$ORIGIN")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/libros2_plugin_proto__rosidl_generator_c.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_generator_c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_generator_c.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_generator_c.so"
         OLD_RPATH "/opt/ros/humble/lib:"
         NEW_RPATH "\$ORIGIN")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_generator_c.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ros2_plugin_proto/ros2_plugin_proto" TYPE DIRECTORY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/rosidl_typesupport_fastrtps_c/ros2_plugin_proto/" REGEX "/[^/]*\\.cpp$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_fastrtps_c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_fastrtps_c.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_fastrtps_c.so"
         RPATH "\$ORIGIN")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/libros2_plugin_proto__rosidl_typesupport_fastrtps_c.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_fastrtps_c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_fastrtps_c.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_fastrtps_c.so"
         OLD_RPATH "/opt/ros/humble/lib:/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto:"
         NEW_RPATH "\$ORIGIN")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_fastrtps_c.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ros2_plugin_proto/ros2_plugin_proto" TYPE DIRECTORY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/rosidl_generator_cpp/ros2_plugin_proto/" REGEX "/[^/]*\\.hpp$")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ros2_plugin_proto/ros2_plugin_proto" TYPE DIRECTORY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/rosidl_typesupport_fastrtps_cpp/ros2_plugin_proto/" REGEX "/[^/]*\\.cpp$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_fastrtps_cpp.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_fastrtps_cpp.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_fastrtps_cpp.so"
         RPATH "\$ORIGIN")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/libros2_plugin_proto__rosidl_typesupport_fastrtps_cpp.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_fastrtps_cpp.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_fastrtps_cpp.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_fastrtps_cpp.so"
         OLD_RPATH "/opt/ros/humble/lib:"
         NEW_RPATH "\$ORIGIN")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_fastrtps_cpp.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ros2_plugin_proto/ros2_plugin_proto" TYPE DIRECTORY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/rosidl_typesupport_introspection_c/ros2_plugin_proto/" REGEX "/[^/]*\\.h$")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_introspection_c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_introspection_c.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_introspection_c.so"
         RPATH "\$ORIGIN")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/libros2_plugin_proto__rosidl_typesupport_introspection_c.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_introspection_c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_introspection_c.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_introspection_c.so"
         OLD_RPATH "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto:/opt/ros/humble/lib:"
         NEW_RPATH "\$ORIGIN")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_introspection_c.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_c.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_c.so"
         RPATH "\$ORIGIN")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/libros2_plugin_proto__rosidl_typesupport_c.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_c.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_c.so"
         OLD_RPATH "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto:/opt/ros/humble/lib:"
         NEW_RPATH "\$ORIGIN")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_c.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ros2_plugin_proto/ros2_plugin_proto" TYPE DIRECTORY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/rosidl_typesupport_introspection_cpp/ros2_plugin_proto/" REGEX "/[^/]*\\.hpp$")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_introspection_cpp.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_introspection_cpp.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_introspection_cpp.so"
         RPATH "\$ORIGIN")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/libros2_plugin_proto__rosidl_typesupport_introspection_cpp.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_introspection_cpp.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_introspection_cpp.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_introspection_cpp.so"
         OLD_RPATH "/opt/ros/humble/lib:"
         NEW_RPATH "\$ORIGIN")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_introspection_cpp.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_cpp.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_cpp.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_cpp.so"
         RPATH "\$ORIGIN")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/libros2_plugin_proto__rosidl_typesupport_cpp.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_cpp.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_cpp.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_cpp.so"
         OLD_RPATH "/opt/ros/humble/lib:"
         NEW_RPATH "\$ORIGIN")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_typesupport_cpp.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/environment" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/ament_cmake_environment_hooks/pythonpath.sh")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/environment" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/ament_cmake_environment_hooks/pythonpath.dsv")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto-0.1.0-py3.11.egg-info" TYPE DIRECTORY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/ament_cmake_python/ros2_plugin_proto/ros2_plugin_proto.egg-info/")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto" TYPE DIRECTORY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/rosidl_generator_py/ros2_plugin_proto/" REGEX "/[^/]*\\.pyc$" EXCLUDE REGEX "/\\_\\_pycache\\_\\_$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  execute_process(
        COMMAND
        "/home/suzumiyaharuhi/.cache/uv/builds-v0/.tmpdGfGQf/bin/python" "-m" "compileall"
        "/tmp/tmp_h9q9_k4/wheel/platlib/lib/python3.11/site-packages/ros2_plugin_proto"
      )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/ros2_plugin_proto__py/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto/ros2_plugin_proto_s__rosidl_typesupport_fastrtps_c.cpython-311-x86_64-linux-gnu.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto/ros2_plugin_proto_s__rosidl_typesupport_fastrtps_c.cpython-311-x86_64-linux-gnu.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto/ros2_plugin_proto_s__rosidl_typesupport_fastrtps_c.cpython-311-x86_64-linux-gnu.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto" TYPE SHARED_LIBRARY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/rosidl_generator_py/ros2_plugin_proto/ros2_plugin_proto_s__rosidl_typesupport_fastrtps_c.cpython-311-x86_64-linux-gnu.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto/ros2_plugin_proto_s__rosidl_typesupport_fastrtps_c.cpython-311-x86_64-linux-gnu.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto/ros2_plugin_proto_s__rosidl_typesupport_fastrtps_c.cpython-311-x86_64-linux-gnu.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto/ros2_plugin_proto_s__rosidl_typesupport_fastrtps_c.cpython-311-x86_64-linux-gnu.so"
         OLD_RPATH "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/rosidl_generator_py/ros2_plugin_proto:/home/suzumiyaharuhi/.local/share/uv/python/cpython-3.11.15-linux-x86_64-gnu/lib:/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto:/opt/ros/humble/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto/ros2_plugin_proto_s__rosidl_typesupport_fastrtps_c.cpython-311-x86_64-linux-gnu.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto/ros2_plugin_proto_s__rosidl_typesupport_introspection_c.cpython-311-x86_64-linux-gnu.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto/ros2_plugin_proto_s__rosidl_typesupport_introspection_c.cpython-311-x86_64-linux-gnu.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto/ros2_plugin_proto_s__rosidl_typesupport_introspection_c.cpython-311-x86_64-linux-gnu.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto" TYPE SHARED_LIBRARY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/rosidl_generator_py/ros2_plugin_proto/ros2_plugin_proto_s__rosidl_typesupport_introspection_c.cpython-311-x86_64-linux-gnu.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto/ros2_plugin_proto_s__rosidl_typesupport_introspection_c.cpython-311-x86_64-linux-gnu.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto/ros2_plugin_proto_s__rosidl_typesupport_introspection_c.cpython-311-x86_64-linux-gnu.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto/ros2_plugin_proto_s__rosidl_typesupport_introspection_c.cpython-311-x86_64-linux-gnu.so"
         OLD_RPATH "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/rosidl_generator_py/ros2_plugin_proto:/home/suzumiyaharuhi/.local/share/uv/python/cpython-3.11.15-linux-x86_64-gnu/lib:/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto:/opt/ros/humble/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto/ros2_plugin_proto_s__rosidl_typesupport_introspection_c.cpython-311-x86_64-linux-gnu.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto/ros2_plugin_proto_s__rosidl_typesupport_c.cpython-311-x86_64-linux-gnu.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto/ros2_plugin_proto_s__rosidl_typesupport_c.cpython-311-x86_64-linux-gnu.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto/ros2_plugin_proto_s__rosidl_typesupport_c.cpython-311-x86_64-linux-gnu.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto" TYPE SHARED_LIBRARY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/rosidl_generator_py/ros2_plugin_proto/ros2_plugin_proto_s__rosidl_typesupport_c.cpython-311-x86_64-linux-gnu.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto/ros2_plugin_proto_s__rosidl_typesupport_c.cpython-311-x86_64-linux-gnu.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto/ros2_plugin_proto_s__rosidl_typesupport_c.cpython-311-x86_64-linux-gnu.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto/ros2_plugin_proto_s__rosidl_typesupport_c.cpython-311-x86_64-linux-gnu.so"
         OLD_RPATH "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/rosidl_generator_py/ros2_plugin_proto:/home/suzumiyaharuhi/.local/share/uv/python/cpython-3.11.15-linux-x86_64-gnu/lib:/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto:/opt/ros/humble/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python3.11/site-packages/ros2_plugin_proto/ros2_plugin_proto_s__rosidl_typesupport_c.cpython-311-x86_64-linux-gnu.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_generator_py.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_generator_py.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_generator_py.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/rosidl_generator_py/ros2_plugin_proto/libros2_plugin_proto__rosidl_generator_py.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_generator_py.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_generator_py.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_generator_py.so"
         OLD_RPATH "/home/suzumiyaharuhi/.local/share/uv/python/cpython-3.11.15-linux-x86_64-gnu/lib:/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto:/opt/ros/humble/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libros2_plugin_proto__rosidl_generator_py.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/msg" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/rosidl_adapter/ros2_plugin_proto/msg/RosMsgWrapper.idl")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/srv" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/rosidl_adapter/ros2_plugin_proto/srv/RosRpcWrapper.idl")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/msg" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/protocols/plugins/ros2_plugin_proto/msg/RosMsgWrapper.msg")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/srv" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/protocols/plugins/ros2_plugin_proto/srv/RosRpcWrapper.srv")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/srv" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/rosidl_cmake/srv/RosRpcWrapper_Request.msg")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/srv" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/rosidl_cmake/srv/RosRpcWrapper_Response.msg")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ament_index/resource_index/package_run_dependencies" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/ament_cmake_index/share/ament_index/resource_index/package_run_dependencies/ros2_plugin_proto")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ament_index/resource_index/parent_prefix_path" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/ament_cmake_index/share/ament_index/resource_index/parent_prefix_path/ros2_plugin_proto")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/environment" TYPE FILE FILES "/opt/ros/humble/share/ament_cmake_core/cmake/environment_hooks/environment/ament_prefix_path.sh")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/environment" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/ament_cmake_environment_hooks/ament_prefix_path.dsv")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/environment" TYPE FILE FILES "/opt/ros/humble/share/ament_cmake_core/cmake/environment_hooks/environment/path.sh")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/environment" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/ament_cmake_environment_hooks/path.dsv")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/ament_cmake_environment_hooks/local_setup.bash")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/ament_cmake_environment_hooks/local_setup.sh")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/ament_cmake_environment_hooks/local_setup.zsh")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/ament_cmake_environment_hooks/local_setup.dsv")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/ament_cmake_environment_hooks/package.dsv")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ament_index/resource_index/packages" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/ament_cmake_index/share/ament_index/resource_index/packages/ros2_plugin_proto")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/export_ros2_plugin_proto__rosidl_generator_cExport.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/export_ros2_plugin_proto__rosidl_generator_cExport.cmake"
         "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/export_ros2_plugin_proto__rosidl_generator_cExport.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/export_ros2_plugin_proto__rosidl_generator_cExport-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/export_ros2_plugin_proto__rosidl_generator_cExport.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/export_ros2_plugin_proto__rosidl_generator_cExport.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/export_ros2_plugin_proto__rosidl_generator_cExport-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/export_ros2_plugin_proto__rosidl_typesupport_fastrtps_cExport.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/export_ros2_plugin_proto__rosidl_typesupport_fastrtps_cExport.cmake"
         "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/export_ros2_plugin_proto__rosidl_typesupport_fastrtps_cExport.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/export_ros2_plugin_proto__rosidl_typesupport_fastrtps_cExport-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/export_ros2_plugin_proto__rosidl_typesupport_fastrtps_cExport.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/export_ros2_plugin_proto__rosidl_typesupport_fastrtps_cExport.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/export_ros2_plugin_proto__rosidl_typesupport_fastrtps_cExport-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/export_ros2_plugin_proto__rosidl_generator_cppExport.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/export_ros2_plugin_proto__rosidl_generator_cppExport.cmake"
         "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/export_ros2_plugin_proto__rosidl_generator_cppExport.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/export_ros2_plugin_proto__rosidl_generator_cppExport-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/export_ros2_plugin_proto__rosidl_generator_cppExport.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/export_ros2_plugin_proto__rosidl_generator_cppExport.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/export_ros2_plugin_proto__rosidl_typesupport_fastrtps_cppExport.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/export_ros2_plugin_proto__rosidl_typesupport_fastrtps_cppExport.cmake"
         "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/export_ros2_plugin_proto__rosidl_typesupport_fastrtps_cppExport.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/export_ros2_plugin_proto__rosidl_typesupport_fastrtps_cppExport-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/export_ros2_plugin_proto__rosidl_typesupport_fastrtps_cppExport.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/export_ros2_plugin_proto__rosidl_typesupport_fastrtps_cppExport.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/export_ros2_plugin_proto__rosidl_typesupport_fastrtps_cppExport-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/ros2_plugin_proto__rosidl_typesupport_introspection_cExport.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/ros2_plugin_proto__rosidl_typesupport_introspection_cExport.cmake"
         "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/ros2_plugin_proto__rosidl_typesupport_introspection_cExport.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/ros2_plugin_proto__rosidl_typesupport_introspection_cExport-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/ros2_plugin_proto__rosidl_typesupport_introspection_cExport.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/ros2_plugin_proto__rosidl_typesupport_introspection_cExport.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/ros2_plugin_proto__rosidl_typesupport_introspection_cExport-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/ros2_plugin_proto__rosidl_typesupport_cExport.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/ros2_plugin_proto__rosidl_typesupport_cExport.cmake"
         "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/ros2_plugin_proto__rosidl_typesupport_cExport.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/ros2_plugin_proto__rosidl_typesupport_cExport-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/ros2_plugin_proto__rosidl_typesupport_cExport.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/ros2_plugin_proto__rosidl_typesupport_cExport.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/ros2_plugin_proto__rosidl_typesupport_cExport-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/ros2_plugin_proto__rosidl_typesupport_introspection_cppExport.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/ros2_plugin_proto__rosidl_typesupport_introspection_cppExport.cmake"
         "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/ros2_plugin_proto__rosidl_typesupport_introspection_cppExport.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/ros2_plugin_proto__rosidl_typesupport_introspection_cppExport-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/ros2_plugin_proto__rosidl_typesupport_introspection_cppExport.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/ros2_plugin_proto__rosidl_typesupport_introspection_cppExport.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/ros2_plugin_proto__rosidl_typesupport_introspection_cppExport-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/ros2_plugin_proto__rosidl_typesupport_cppExport.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/ros2_plugin_proto__rosidl_typesupport_cppExport.cmake"
         "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/ros2_plugin_proto__rosidl_typesupport_cppExport.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/ros2_plugin_proto__rosidl_typesupport_cppExport-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/ros2_plugin_proto__rosidl_typesupport_cppExport.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/ros2_plugin_proto__rosidl_typesupport_cppExport.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/ros2_plugin_proto__rosidl_typesupport_cppExport-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/export_ros2_plugin_proto__rosidl_generator_pyExport.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/export_ros2_plugin_proto__rosidl_generator_pyExport.cmake"
         "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/export_ros2_plugin_proto__rosidl_generator_pyExport.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/export_ros2_plugin_proto__rosidl_generator_pyExport-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake/export_ros2_plugin_proto__rosidl_generator_pyExport.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/export_ros2_plugin_proto__rosidl_generator_pyExport.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/CMakeFiles/Export/0dfc3a0c1bf0e1d58ed4d4ec6220794c/export_ros2_plugin_proto__rosidl_generator_pyExport-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/rosidl_cmake/rosidl_cmake-extras.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/ament_cmake_export_include_directories/ament_cmake_export_include_directories-extras.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/ament_cmake_export_libraries/ament_cmake_export_libraries-extras.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/ament_cmake_export_targets/ament_cmake_export_targets-extras.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/rosidl_cmake/rosidl_cmake_export_typesupport_targets-extras.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/ament_cmake_export_dependencies/ament_cmake_export_dependencies-extras.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/rosidl_cmake/rosidl_cmake_export_typesupport_libraries-extras.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto/cmake" TYPE FILE FILES
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/ament_cmake_core/ros2_plugin_protoConfig.cmake"
    "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/ament_cmake_core/ros2_plugin_protoConfig-version.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ros2_plugin_proto" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src/src/protocols/plugins/ros2_plugin_proto/package.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/libaimrt_protocols_ros2_plugin_proto_aimrt_rpc_gencode.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aimrt_protocols_ros2_plugin_proto_aimrt_rpc_gencode" TYPE FILE FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/RosRpcWrapper.aimrt_rpc.srv.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libros2_plugin_proto_ros2_ts.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libros2_plugin_proto_ros2_ts.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libros2_plugin_proto_ros2_ts.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto/libros2_plugin_proto_ros2_ts.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libros2_plugin_proto_ros2_ts.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libros2_plugin_proto_ros2_ts.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libros2_plugin_proto_ros2_ts.so"
         OLD_RPATH "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build/src/protocols/plugins/ros2_plugin_proto:/opt/ros/humble/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libros2_plugin_proto_ros2_ts.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

