cmake_minimum_required(VERSION 3.22)
project(docker-cpp)
set( CMAKE_CXX_FLAGS "-std=c++17" )

include_directories(${CMAKE_SOURCE_DIR})


file(GLOB SOURCES main.cpp "cgroup/*.cpp" "config/*.cpp" "child/*.cpp")

add_subdirectory(../sheep-tools sheep-tools)

add_executable(docker-cpp ${SOURCES})

target_link_libraries(docker-cpp sheeptools)