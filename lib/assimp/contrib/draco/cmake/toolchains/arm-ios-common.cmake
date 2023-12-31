# Copyright 2021 The Draco Authors
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy of
# the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.

if(DRACO_CMAKE_TOOLCHAINS_ARM_IOS_COMMON_CMAKE_)
  return()
endif()
set(DRACO_CMAKE_ARM_IOS_COMMON_CMAKE_ 1)

set(CMAKE_SYSTEM_NAME "Darwin")
if(CMAKE_OSX_SDK)
  set(CMAKE_OSX_SYSROOT ${CMAKE_OSX_SDK})
else()
  set(CMAKE_OSX_SYSROOT iphoneos)
endif()
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_COMPILER_ARG1 "-arch ${CMAKE_SYSTEM_PROCESSOR}")
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_ARG1 "-arch ${CMAKE_SYSTEM_PROCESSOR}")
