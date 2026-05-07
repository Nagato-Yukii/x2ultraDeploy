# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/libunifex-src"
  "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/libunifex-build"
  "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/libunifex-subbuild/libunifex-populate-prefix"
  "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/libunifex-subbuild/libunifex-populate-prefix/tmp"
  "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/libunifex-subbuild/libunifex-populate-prefix/src/libunifex-populate-stamp"
  "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/libunifex-subbuild/libunifex-populate-prefix/src"
  "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/libunifex-subbuild/libunifex-populate-prefix/src/libunifex-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/libunifex-subbuild/libunifex-populate-prefix/src/libunifex-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/suzumiyaharuhi/rl_deploy_v2/deploy/build/legged_system/_deps/libunifex-subbuild/libunifex-populate-prefix/src/libunifex-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
