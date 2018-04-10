cmake_minimum_required(VERSION 3.0)
project(oms CXX)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_BUILD_TYPE Debug)

if (POLICY CMP0003)
    cmake_policy(SET CMP0003 NEW)
endif ()

if (POLICY CMP0042)
    cmake_policy(SET CMP0042 NEW)
endif ()

if (POLICY CMP0064)
    cmake_policy(SET CMP0064 NEW)
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow")
set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath,'$ORIGIN/jdk8m/lib/amd64/server/'")

find_package(Boost REQUIRED)

find_library(BOOST_chrono_LIBRARY NAMES libboost_chrono.a libboost_chrono-mt.a)
find_library(BOOST_date_time_LIBRARY NAMES libboost_date_time.a libboost_date_time-mt.a)
find_library(BOOST_thread_LIBRARY NAMES libboost_thread.a libboost_thread-mt.a)
find_library(BOOST_filesystem_LIBRARY NAMES libboost_filesystem.a libboost_filesystem-mt.a)
find_library(BOOST_system_LIBRARY NAMES libboost_system.a libboost_system-mt.a)

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/sdk/include)

if (UNIX AND NOT APPLE)
    set(JNI_INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/third_party/jdk8m/include
        ${CMAKE_SOURCE_DIR}/third_party/jdk8m/include/linux)
    set(JAVA_JVM_LIBRARY
        ${CMAKE_SOURCE_DIR}/third_party/jdk8m/lib/amd64/server/libjvm.so)
else()
    find_package(JNI REQUIRED)
endif ()

include_directories(SYSTEM ${JNI_INCLUDE_DIRS})

add_subdirectory(sdk)

option(BUILD_EXAMPLE "Build examples flag" OFF)

if (BUILD_EXAMPLE)
    add_subdirectory(example)
endif ()

option(TEST "Build test cases" ON)
if(TEST)
    enable_testing()
    option(gtest_build_tests OFF)
    add_subdirectory(third_party/googletest/googletest)
    include_directories(SYSTEM ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
    add_subdirectory(tests)
endif()

option(BENCHMARK "Build benchmark test cases" OFF)
if (BENCHMARK)
    add_subdirectory(third_party/benchmark)
    include_directories(SYSTEM third_party/benchmark/include)
    add_subdirectory(benchmark)
endif ()
