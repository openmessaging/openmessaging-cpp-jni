cmake_minimum_required(VERSION 3.0)
project(oms CXX)

set(CMAKE_CXX_STANDARD 98)

if (COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
    cmake_policy(SET CMP0042 NEW)
    cmake_policy(SET CMP0064 NEW)
endif (COMMAND cmake_policy)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow")

# set(JAVA_HOME "/Library/Java/JavaVirtualMachines/jdk1.8.0_144.jdk/Contents/Home")
find_package(JNI REQUIRED)
message("Java include path: ${JAVA_INCLUDE_PATH}")

find_package(Boost REQUIRED)

include_directories(
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/sdk/include
        ${JNI_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS})

add_subdirectory(sdk)

option(BUILD_EXAMPLE "Build examples flag" ON)

if (BUILD_EXAMPLE)
    add_subdirectory(example)
endif ()

option(TEST "Build test cases" ON)
if(TEST)
    enable_testing()
    option(gtest_build_tests OFF)
    add_subdirectory(third_party/googletest/googletest)
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
    add_subdirectory(tests)
endif()

option(BENCHMARK "Build benchmark test cases" ON)
if (BENCHMARK)
    add_subdirectory(third_party/benchmark)
    include_directories(third_party/benchmark/include)
    add_subdirectory(benchmark)
endif ()
