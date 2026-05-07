# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/iceoryx-src"
  "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/iceoryx-build"
  "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/iceoryx-subbuild/iceoryx-populate-prefix"
  "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/iceoryx-subbuild/iceoryx-populate-prefix/tmp"
  "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/iceoryx-subbuild/iceoryx-populate-prefix/src/iceoryx-populate-stamp"
  "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/iceoryx-subbuild/iceoryx-populate-prefix/src"
  "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/iceoryx-subbuild/iceoryx-populate-prefix/src/iceoryx-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/iceoryx-subbuild/iceoryx-populate-prefix/src/iceoryx-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/iceoryx-subbuild/iceoryx-populate-prefix/src/iceoryx-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
