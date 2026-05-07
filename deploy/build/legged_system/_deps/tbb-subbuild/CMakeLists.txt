# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.25.0)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(tbb-populate NONE)



include(ExternalProject)
ExternalProject_Add(tbb-populate
                     "UPDATE_DISCONNECTED" "False" "DOWNLOAD_EXTRACT_TIMESTAMP" "TRUE" "URL" "https://github.com/oneapi-src/oneTBB/archive/v2021.13.0.tar.gz"
                    SOURCE_DIR          "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/tbb-src"
                    BINARY_DIR          "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/tbb-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
                    USES_TERMINAL_PATCH     YES
)


