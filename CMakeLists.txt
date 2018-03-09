cmake_minimum_required(VERSION 3.0)
project(oms CXX)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_BUILD_TYPE Debug)

if (COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
    cmake_policy(SET CMP0042 NEW)
    cmake_policy(SET CMP0064 NEW)
endif (COMMAND cmake_policy)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow")
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath,'$ORIGIN/jdk8m/lib/amd64/server/'")

find_package(JNI REQUIRED)
message("Java include path: ${JAVA_INCLUDE_PATH}")
message("JAVA_JVM_LIBRARY: ${JAVA_JVM_LIBRARY}")
message("JNI_LIBRARIES: ${JNI_LIBRARIES}")
link_directories(/home/shutian.lzh/oms/jdk8m/lib/amd64 /home/shutian.lzh/oms/jdk8m/lib/amd64/server)

find_package(Boost REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/sdk/include)

include_directories(SYSTEM ${JNI_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

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
    include_directories(SYSTEM ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
    add_subdirectory(tests)
endif()

option(BENCHMARK "Build benchmark test cases" OFF)
if (BENCHMARK)
    add_subdirectory(third_party/benchmark)
    include_directories(SYSTEM third_party/benchmark/include)
    add_subdirectory(benchmark)
endif ()
