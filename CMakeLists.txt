cmake_minimum_required(VERSION 3.20.1)

set(this_lib gmultifi_client-vsomeip)

# project name
project(${this_lib} LANGUAGES CXX)

find_library(ZENOH_LIB zenohc)
message("ZENOH_LIB: ${ZENOH_LIB}")
find_package(up-cpp REQUIRED)
find_package(vsomeip3 REQUIRED)
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

add_library(gmultifi_client-vsomeip::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

#TODO: -fPIC and -Werror needs to be added later. Currently it is causing issues because uTransport throws error.
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

target_link_libraries(${this_lib} PUBLIC
    rapidjson
    protobuf::libprotobuf
    fmt::fmt
    spdlog::spdlog
    vsomeip3::vsomeip3
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
