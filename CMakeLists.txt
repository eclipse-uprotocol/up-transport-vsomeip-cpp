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

cmake_minimum_required(VERSION 3.20.1)

# project name
project(${PROJECT_NAME} LANGUAGES CXX)

if(DEFINED ENV{LOCAL_VSOMEIP_PATH})
    message("LOCAL_VSOMEIP_PATH [$ENV{LOCAL_VSOMEIP_PATH}]")
    include_directories($ENV{LOCAL_VSOMEIP_PATH}/include)
    link_directories($ENV{LOCAL_VSOMEIP_PATH}/lib)
    add_link_options(-Wl,--no-as-needed)
    set(VSOMEIP_LIB vsomeip3)
else()
    find_package(vsomeip3 REQUIRED)
    set(VSOMEIP_LIB vsomeip3::vsomeip3)
endif()

find_package(up-cpp REQUIRED)
find_package(Boost REQUIRED)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED CONFIG)
find_package(protobuf CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
add_definitions(-DSPDLOG_FMT_EXTERNAL)


message("up-cpp_INCLUDE_DIR = ${up-cpp_INCLUDE_DIR}")
include_directories(include)
include_directories(${up-cpp_INCLUDE_DIR})

add_library(
    ${PROJECT_NAME} SHARED
    src/VsomeipUTransport.cpp
    src/Logger.cpp
    src/SomeipRouter.cpp
    src/SomeipWrapper.cpp
    src/SomeipHandler.cpp
    src/MessageTranslator.cpp)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_options(${PROJECT_NAME} PRIVATE
    -O2
    #-fPIC
    -g
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

target_link_libraries(${PROJECT_NAME} PUBLIC
    rapidjson
    protobuf::libprotobuf
    fmt::fmt
    spdlog::spdlog
    ${VSOMEIP_LIB}
    Boost::boost
    Boost::system
    Boost::thread
    Boost::filesystem)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
endif()

INSTALL(TARGETS ${PROJECT_NAME})
INSTALL(DIRECTORY include DESTINATION .)
