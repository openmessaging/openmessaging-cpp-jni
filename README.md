## Build Status

![Status](https://travis-ci.org/lizhanhui/openmessaging-cpp.svg?branch=develop)

## Introduction
This project contains two modules: OpenMessaging C++ Specification API and JNI-based implementation.

For the first part, we are intending to maintain conformance to Java counterpart. 

For the latter one, a proxy is planned to implement, embedding a JVM and delegating all invocation to Java SDK. Make 
sure you have thoroughly understood JNI technique before contributing. [JNI Programmer Guide](doc/JNI_guide.pdf) is by 
far the best resource to consult.


## Documentation

1. [Application Design Specification](doc/ApplicationDesignSpecification.md)
2. [Avoid Debugging Pitfalls Guide](doc/Debug.md)
3. [JNI Programmer Guide](doc/JNI_guide.pdf)

## Prerequisite 
1. CMake 3.0+
2. C++ Toolchain that supports C++98 or later.
3. Boost

## How to build

1. Please first clone this project to a local directory. Remember to recursively clone all the submodules.
2. Execute ci.sh in the source folder.