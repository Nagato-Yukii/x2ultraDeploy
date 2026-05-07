# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/protobuf-src"
  "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/protobuf-build"
  "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/protobuf-subbuild/protobuf-populate-prefix"
  "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/protobuf-subbuild/protobuf-populate-prefix/tmp"
  "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/protobuf-subbuild/protobuf-populate-prefix/src/protobuf-populate-stamp"
  "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/protobuf-subbuild/protobuf-populate-prefix/src"
  "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/protobuf-subbuild/protobuf-populate-prefix/src/protobuf-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/protobuf-subbuild/protobuf-populate-prefix/src/protobuf-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/protobuf-subbuild/protobuf-populate-prefix/src/protobuf-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
