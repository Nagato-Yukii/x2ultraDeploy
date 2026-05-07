# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.25.0)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(cpptoml-populate NONE)



include(ExternalProject)
ExternalProject_Add(cpptoml-populate
                     "UPDATE_DISCONNECTED" "False" "DOWNLOAD_EXTRACT_TIMESTAMP" "ON" "URL" "https://github.com/skystrife/cpptoml/archive/refs/tags/v0.1.0.tar.gz"
                    SOURCE_DIR          "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/cpptoml-src"
                    BINARY_DIR          "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/cpptoml-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
                    USES_TERMINAL_PATCH     YES
)


