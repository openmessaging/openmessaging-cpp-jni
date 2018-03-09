## Design

   We are building a thin layer shared C/C++ library, allowing C/C++ users to make best of existing Java client SDK
    with ease. This brings about three significant strengths: 
    
   1. Application developers are accessible to latest features which were available to Java SDK at first and then 
   propagated to other languages later on; 
    
   2. We, the library developers, only need to maintain core client business code at one place; 
    
   3. It's basically even cheaper to build SDKs for the
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
1. Core Boost libraries are employed and statically linked;
1. Avoid raw pointer if possible, boost smart_ptr should be used instead. As a result, it's OK to introduce
 dependency on Boost.smart_ptr in public header files;
1. GoogleTest for unit tests;
1. Google micro-benchmark library for core code path;

## Runtime shared library loading

1. To further enhance convenience of replacing one OMS implementation with another, a plugin mechanism is adopted on top
of dl-series system functions. OMS vendors need to implement a few C-linkage style methods defined in `MessagingAccessPointFactory.h`

2. Due to features mentioned above, application developer does not need to link their application against any vendor 
specific library; Instead, as long as `ld` may find the implementing library specified in access point url, executables 
will run without glitches.

## Thread-Safety and Concurrency

1. Core sending / receiving interfaces are guaranteed thread-safe;

## Exception Handling

1. Java exceptions should be checked and cleared from C++ side;
2. Exceptions, with identical semantics should be created in C++;

## Logging

1. An efficient logging library is expected, as should conform to C++ 98 standard. At present, 
[plog](https://github.com/SergiusTheBest/plog) is adopted and included in the SDK module.

2. Key execution path of pubic interface methods are supposed to to logged down employing various levels, in hope of 
diagnosing any potential problem. 

3. On exception, error level should be used. In case there were Java exception involved, complete stack trace should be
included.

## Deployment Constraints

1. For rpm/deb-based operating systems, we distribute this library through package manager;
2. Other Linux OS are supposed to build from source code;
3. Windows users are provided with pre-built dll files along with header files;