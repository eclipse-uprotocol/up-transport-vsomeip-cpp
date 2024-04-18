cmake_minimum_required (VERSION 2.8.7)
project (mE_publisher)

find_package(Threads REQUIRED)

set(VSOMEIP_NAME "vsomeip3")


#set (CMAKE_CXX_FLAGS "-g -std=c++0x")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
message("VSOMEIP_INCLUDE_DIRS = ${VSOMEIP_INCLUDE_DIRS}")
include_directories(${VSOMEIP_INCLUDE_DIRS})

find_package(${VSOMEIP_NAME})
if (NOT ${VSOMEIP_NAME}_FOUND)
    message("${VSOMEIP_NAME} was not found. Please specify vsomeip_DIR")
endif()

#mE_publisher
add_executable (mE_publisher mE_publisher.cpp)
message("VSOMEIP_LIBRARIES=${VSOMEIP_LIBRARIES}")
target_link_libraries(mE_publisher ${VSOMEIP_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

#mE_rpcServer
add_executable (mE_rpcServer mE_rpcServer.cpp)
message("VSOMEIP_LIBRARIES=${VSOMEIP_LIBRARIES}")
target_link_libraries(mE_rpcServer ${VSOMEIP_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

#mE_rpcClient
add_executable (mE_rpcClient mE_rpcClient.cpp)
message("VSOMEIP_LIBRARIES=${VSOMEIP_LIBRARIES}")
target_link_libraries(mE_rpcClient ${VSOMEIP_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

#mE_subscriber
add_executable (mE_subscriber mE_subscriber.cpp)
message("VSOMEIP_LIBRARIES=${VSOMEIP_LIBRARIES}")
target_link_libraries(mE_subscriber ${VSOMEIP_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
