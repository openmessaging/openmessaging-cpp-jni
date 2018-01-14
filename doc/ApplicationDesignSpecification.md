## Design
    We are building a thin layer shared C/C++ library, allowing C/C++ users to make best of existing Java client SDK
    with ease. This brings about three significat strengths: 1) Application developers are accessible to latest features 
    which were available to Java SDK at first and propogated to other languages later on; 2) We, the library developers,
    only need to maintain core client business code at one place; 3) It's basically even cheaper to build SDKs for the
    rest languages on top of this thin library.
    
## Overview

1. A proof of concept library has been built. See [Java Sample Code](http://gitlab.alibaba-inc.com/messaging/babel-java)
   and [CPP Sample Code](http://gitlab.alibaba-inc.com/messaging/babel-cpp)
   
1. Calling Java code from C++ library has been micro-bench-marked, guaranteeing that overhead brought about is acceptable
   for clients. Please examine the benchmark folder of the CPP repository carefully. 
   
1. This project is divided into three sub-projects, each of which has specific goals as follows:
   * openmessaging-cpp sub-module, which intends to provided C++ specific interfaces in correspondence with Java
     counterparts;
     
   * rocketmq-openmessaging-wrapper sub-module, which is to convert all Java Future/Promise, Callback methods into
     calls into native methods, as are programmatically bound to native functions in C/C++ library;
     
   * openmessaging-bridge sub-module, which implements all interfaces defined in openmessaging-cpp through leveraging 
     existing Java Client SDK;

## Technical Specification

1. C++ codes conforms to C++98 language standard;
1. C++ interfaces should conform to Java counterparts as much as possible;
1. Boost core modules are preferred;
1. Avoid raw pointer if possible;
1. GoogleTest for unit tests;
1. Google micro-benchmark library for core code path;

## Thread-Safety and Concurrency

1. Core sending / receiving interfaces are guaranteed thread-safe;

## Exception Handling

1. Java exceptions should be checked and cleared from C++ side;
2. Exceptions, with identical semantics should be created in C++;

## Logging

1. We are supposed to have an internal logging system, helping us diagnose all potential issues.
2. For the sake C++98 standard, Boost.log is used for now.

## Deployment Constraints

1. For rpm/deb-based operating systems, we distribute this library through package manager;
2. Other Linux OS are supposed to build from source code;
3. Windows users are provided with pre-built dll files along with header files;