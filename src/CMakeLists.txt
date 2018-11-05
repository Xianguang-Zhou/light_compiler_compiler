# Copyright (c) 2018, Xianguang Zhou <xianguang.zhou@outlook.com>. All rights reserved.
# 
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

project(light_compiler_compiler)
cmake_minimum_required(VERSION 3.1)

find_package(Boost REQUIRED COMPONENTS regex)
add_definitions(${Boost_DEFINITIONS})
include_directories(${BOOST_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

aux_source_directory(. SRC_LIST)

set(EXECUTABLE_NAME "light-compiler-compiler")
add_executable(${EXECUTABLE_NAME} ${SRC_LIST})
target_link_libraries(${EXECUTABLE_NAME} ${Boost_REGEX_LIBRARY})

