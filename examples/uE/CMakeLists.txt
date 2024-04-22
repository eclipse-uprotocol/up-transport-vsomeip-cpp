# Copyright (c) 2024 General Motors GTO LLC
#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: 2024 General Motors GTO LLC
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20)
project(uE VERSION 0.1.0 LANGUAGES CXX)
set(SOMEIP_CONFIG_FILE "someip-config.json")

if(DEFINED ENV{LOCAL_VSOMEIP_PATH})
    message("LOCAL_VSOMEIP_PATH [$ENV{LOCAL_VSOMEIP_PATH}]")
    include_directories($ENV{LOCAL_VSOMEIP_PATH}/include)
    link_directories($ENV{LOCAL_VSOMEIP_PATH}/lib)
    # add_link_options(-Wl,--no-as-needed)
#     set(VSOMEIP_LIB vsomeip3)
# else()
#     find_package(vsomeip3 REQUIRED)
#     set(VSOMEIP_LIB vsomeip3::vsomeip3)
endif()


find_package(spdlog REQUIRED)
find_package(up-cpp QUIET)
find_package(up-client-vsomeip-cpp REQUIRED)
find_package(fmt REQUIRED CONFIG)
find_package(spdlog REQUIRED)
find_package(protobuf CONFIG REQUIRED)
add_definitions(-DSPDLOG_FMT_EXTERNAL)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../../config/${SOMEIP_CONFIG_FILE} ${CMAKE_BINARY_DIR}/${SOMEIP_CONFIG_FILE} COPYONLY)

#uE_subscriber
add_executable(uE_subscriber uE_subscriber.cpp)
target_link_libraries(uE_subscriber PRIVATE
        up-cpp::up-cpp
        up-client-vsomeip-cpp::up-client-vsomeip-cpp
        fmt::fmt
        spdlog::spdlog
        protobuf::libprotobuf
    )
target_compile_options(uE_subscriber PRIVATE
        -O2
        -g
        -fPIC
        -Wall
        -Wextra
        -pedantic
        -Werror
        -Wcast-qual
        -Wunused
        -Wuninitialized
        -Wmissing-declarations
        -Wconversion
        -Wpointer-arith
        -Wshadow
        -Wfloat-equal
    )
target_include_directories(uE_subscriber
    PUBLIC
        ${fmt_INCLUDE_DIR}
        ${spdlog_INCLUDE_DIR}
        ${up-cpp_INCLUDE_DIR})

target_include_directories(uE_subscriber
    PRIVATE
        include)


#uE_rpcClient
add_executable(uE_rpcClient uE_rpcClient.cpp)
target_link_libraries(uE_rpcClient PRIVATE
        up-cpp::up-cpp
        up-client-vsomeip-cpp::up-client-vsomeip-cpp
        fmt::fmt
        spdlog::spdlog
        protobuf::libprotobuf
    )
target_compile_options(uE_rpcClient PRIVATE
        -O2
        -g
        -fPIC
        -Wall
        -Wextra
        -pedantic
        -Werror
        -Wcast-qual
        -Wunused
        -Wuninitialized
        -Wmissing-declarations
        -Wconversion
        -Wpointer-arith
        -Wshadow
        -Wfloat-equal
    )
target_include_directories(uE_rpcClient
    PUBLIC
        ${fmt_INCLUDE_DIR}
        ${spdlog_INCLUDE_DIR}
        ${up-cpp_INCLUDE_DIR})

target_include_directories(uE_rpcClient
    PRIVATE
        include)


#uE_rpcServer
add_executable(uE_rpcServer uE_rpcServer.cpp)
target_link_libraries(uE_rpcServer PRIVATE
        up-cpp::up-cpp
        up-client-vsomeip-cpp::up-client-vsomeip-cpp
        fmt::fmt
        spdlog::spdlog
        protobuf::libprotobuf
    )
target_compile_options(uE_rpcServer PRIVATE
        -O2
        -g
        -fPIC
        -Wall
        -Wextra
        -pedantic
        -Werror
        -Wcast-qual
        -Wunused
        -Wuninitialized
        -Wmissing-declarations
        -Wconversion
        -Wpointer-arith
        -Wshadow
        -Wfloat-equal
    )
target_include_directories(uE_rpcServer
    PUBLIC
        ${fmt_INCLUDE_DIR}
        ${spdlog_INCLUDE_DIR}
        ${up-cpp_INCLUDE_DIR})

target_include_directories(uE_rpcServer
    PRIVATE
        include)


#uE_publisher
add_executable(uE_publisher uE_publisher.cpp)
target_link_libraries(uE_publisher PRIVATE
        up-cpp::up-cpp
        up-client-vsomeip-cpp::up-client-vsomeip-cpp
        fmt::fmt
        spdlog::spdlog
        protobuf::libprotobuf
    )
target_compile_options(uE_publisher PRIVATE
        -O2
        -g
        -fPIC
        -Wall
        -Wextra
        -pedantic
        -Werror
        -Wcast-qual
        -Wunused
        -Wuninitialized
        -Wmissing-declarations
        -Wconversion
        -Wpointer-arith
        -Wshadow
        -Wfloat-equal
    )
target_include_directories(uE_publisher
    PUBLIC
        ${fmt_INCLUDE_DIR}
        ${spdlog_INCLUDE_DIR}
        ${up-cpp_INCLUDE_DIR})

target_include_directories(uE_publisher
    PRIVATE
        include)