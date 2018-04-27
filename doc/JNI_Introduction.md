## Introduction to Java Native Interface

This documentation illustrates how to program on top of JNI _Invocation API_ so that native C/C++ code may call
Java code in the same operation system process.

## Target Audience

This documentation is for those who are new to JNI Invocation API.

## Prerequisites

Please install the following software before coding:
1. C/C++ Tool Chain: GCC, Clang etc.
2. [CMake 3.0+](https://cmake.org/)
3. [Java Developer Kit](http://www.oracle.com/technetwork/java/javase/downloads/index.html)

## Chapter 1: Launch and Shutdown JVM

#### Create file ch01_1.cpp with the following content

```cpp
#include <iostream>
#include <jni.h>

using namespace std;

int main(int argc, char* argv[]) {
    JavaVM *vm;
    JNIEnv *env;
    JavaVMInitArgs vm_args;
    JavaVMOption* options = new JavaVMOption[3];
    options[0].optionString = const_cast<char*>("-Djava.class.path=.");
    vm_args.version = JNI_VERSION_1_6;
    vm_args.nOptions = 1;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = false;

    JNI_CreateJavaVM (&vm, (void**)&env, &vm_args);
    delete[] options;
    
    jint version = env->GetVersion();
    cout << "Version: " << ((version >> 16) & 0xFF) << "." << (version & 0xFF);
    vm->DestroyJavaVM();   
}
```

#### Create a CMakeLists.txt file

```cmake
cmake_minimum_required(VERSION 2.8)

project(JNI_Introduction)
set(CMAKE_BUILD_TYPE Debug)

find_package(JNI REQUIRED)
INCLUDE_DIRECTORIES(${JNI_INCLUDE_DIRS})

add_executable(ch01_1 "ch01_1.cpp")
target_link_libraries(ch01_1 ${JAVA_JVM_LIBRARY})

```
#### Build and Run
Execute the following commands to build and run your executable file:
```commandline
mkdir build
cd build
cmake ..
make
./ch01_1
```

Output should be similar to the following text:

```text
$cmake ..
-- The C compiler identification is GNU 4.8.5
-- The CXX compiler identification is GNU 4.8.5
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Found JNI: /home/shutian.lzh/jdk1.8.0_144/jre/lib/amd64/libjawt.so  
-- Configuring done
-- Generating done
-- Build files have been written to: /home/shutian.lzh/tutorial/build

[shutian.lzh@e010125053231.bja /home/shutian.lzh/tutorial/build]
$make
Scanning dependencies of target ch01_1
[ 50%] Building CXX object CMakeFiles/ch01_1.dir/ch01_1.cpp.o
[100%] Linking CXX executable ch01_1
[100%] Built target ch01_1

[shutian.lzh@e010125053231.bja /home/shutian.lzh/tutorial/build]
$./ch01_1 
Version: 1.8
```

#### Code Explained

Before launching a Java virtual machine, we need to create a **JavaVMInitArgs** object, which 
holds arguments to pass to virtual machine. Important members of this struct are minimum version of JVM
it accepts, number of options and a pointer to the option array.

After preparing virtual machine arguments, we can now call **JNI_CreateJavaVM** function, which would help
us launch a virtual machine. After invocation, **vm** points to the created VM; **env** points to a context
which will be used as a handle to interact with VM for current thread; **vm_args** points to the initial VM
 arguments.
 
 ``jint version = env->GetVersion();`` gets the version of the launched virtual machine. To convert to canonical
 representation of x.y, shift then *and* against 0xFF as exemplified.
 
 To shutdown the running virtual machine, Calling ``vm->DestroyJavaVM(); `` suffices.
 
 #### One More Thing
 Oracle Hotspot Java Virtual Machine currently allows only one VM instance for each process. Even if you have already
 destroyed the previous one, you, still, may NOT launch a new one.