#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "rl_controllers::rl_controllers" for configuration ""
set_property(TARGET rl_controllers::rl_controllers APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(rl_controllers::rl_controllers PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_NOCONFIG "onnxruntime::onnxruntime"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/librl_controllers.so"
  IMPORTED_SONAME_NOCONFIG "librl_controllers.so"
  )

list(APPEND _cmake_import_check_targets rl_controllers::rl_controllers )
list(APPEND _cmake_import_check_files_for_rl_controllers::rl_controllers "${_IMPORT_PREFIX}/lib/librl_controllers.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
