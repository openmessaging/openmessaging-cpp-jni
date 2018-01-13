cmake_minimum_required(VERSION 3.0)
project(oms CXX)

set(CMAKE_CXX_STANDARD 98)

if (COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
    cmake_policy(SET CMP0042 NEW)
    cmake_policy(SET CMP0064 NEW)
endif (COMMAND cmake_policy)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow")

find_package(JNI REQUIRED)

find_package(Boost REQUIRED)

include_directories(
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/sdk/include
        ${JNI_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS})

add_subdirectory(sdk)

add_subdirectory(example)

option(TEST "Build test cases" ON)
if(TEST)
    enable_testing()
    option(gtest_build_tests OFF)
    add_subdirectory(third_party/googletest/googletest)
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
    add_subdirectory(tests)
endif()

option(BENCHMARK "Build benchmark test cases" OFF)
if (BENCHMARK)
    option(BENCHMARK_ENABLE_TESTING OFF)
    add_subdirectory(lib/benchmark)
endif ()
