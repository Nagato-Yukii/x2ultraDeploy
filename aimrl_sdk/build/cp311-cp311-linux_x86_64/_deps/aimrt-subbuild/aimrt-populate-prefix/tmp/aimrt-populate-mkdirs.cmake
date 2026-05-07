# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-src"
  "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-build"
  "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-subbuild/aimrt-populate-prefix"
  "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-subbuild/aimrt-populate-prefix/tmp"
  "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-subbuild/aimrt-populate-prefix/src/aimrt-populate-stamp"
  "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-subbuild/aimrt-populate-prefix/src"
  "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-subbuild/aimrt-populate-prefix/src/aimrt-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-subbuild/aimrt-populate-prefix/src/aimrt-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/suzumiyaharuhi/rl_deploy_v2/aimrl_sdk/build/cp311-cp311-linux_x86_64/_deps/aimrt-subbuild/aimrt-populate-prefix/src/aimrt-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
