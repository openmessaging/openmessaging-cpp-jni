## Target Audience

This documentation is for those who are new to JNI Invocation API.

## Chapter 2: Run Java Code from Native Code

In the previous chapter, we manage to launch an embedded Java VM and inquire its version. However, we
have not executed any Java code yet.

The sequel section demonstrates this aspect.

#### Create a Java class with a static method

Create file Ch02_1.java

```java

public class Ch02_1 {

    public static void fun() {
        System.out.println("Hello JNI");
    }
}

```

#### Call Java Code from C++

**Create file ch02_1.cpp**
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
    
    jclass klass = env->FindClass("Ch02_1");
    if (NULL == klass) {
        cout << "Forgot to compile your Java code? Cannot find Ch02_1.class in the current directory" << endl;
        return -1;
    }
    jmethodID midFun = env->GetStaticMethodID(klass, "fun", "()V");
    env->CallStaticVoidMethod(klass, midFun);
    
    jint version = env->GetVersion();
    cout << "Version: " << ((version >> 16) & 0xFF) << "." << (version & 0xFF);
    vm->DestroyJavaVM();   
}
```

**Update CMakeLists.txt**

Update CMakeLists.txt to compile and link the new program

```cmake
cmake_minimum_required(VERSION 2.8)

project(JNI_Introduction)
set(CMAKE_BUILD_TYPE Debug)

find_package(JNI REQUIRED)
INCLUDE_DIRECTORIES(${JNI_INCLUDE_DIRS})

add_executable(ch01_1 "ch01_1.cpp")
target_link_libraries(ch01_1 ${JAVA_JVM_LIBRARY})

add_executable(ch02_1 "ch02_1.cpp")
target_link_libraries(ch02_1 ${JAVA_JVM_LIBRARY})

```

#### Compile and Run
```commandline
cd build
cmake ..
make
javac ../Ch02_1.java -d .
./ch02_1
```

Example output is as follows:
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
Scanning dependencies of target ch02_1
[ 25%] Building CXX object CMakeFiles/ch02_1.dir/ch02_1.cpp.o
[ 50%] Linking CXX executable ch02_1
[ 50%] Built target ch02_1
Scanning dependencies of target ch01_1
[ 75%] Building CXX object CMakeFiles/ch01_1.dir/ch01_1.cpp.o
[100%] Linking CXX executable ch01_1
[100%] Built target ch01_1

[shutian.lzh@e010125053231.bja /home/shutian.lzh/tutorial/build]
$javac ../Ch02_1.java -d .

[shutian.lzh@e010125053231.bja /home/shutian.lzh/tutorial/build]
$./ch02_1 
Hello JNI
Version: 1.8
```
#### Code Explained

Most of lines in ch02_1.cpp are exactly same to ch01_1.cpp. As a result, we'd examine 
the added parts:
```cpp
    jclass klass = env->FindClass("Ch02_1");
    if (NULL == klass) {
        cout << "Forgot to compile your Java code? Cannot find Ch02_1.class in the current directory" << endl;
        return -1;
    }
    jmethodID midFun = env->GetStaticMethodID(klass, "fun", "()V");
    env->CallStaticVoidMethod(klass, midFun);
``` 

``jclass klass = env->FindClass("Ch02_1");`` requests embedded JVM to load class specified.
Qualified class name is used, separated by '/'. For example, if we were to load class java.lang.String, we need
to ```jclass klassString = env->FindClass("java/lang/String");``` If the class loader cannot find it,
NULL will be returned.

Like Java Reflection API, before invoking a method, we need to acquire its method ID. In Invocation API,
`GetStaiticMethodID` and `GetMethodID` is employed for static and member method respectively. Both of them
takes 3 parameters: class, method name, and [method signature](https://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/types.html).
Take **()V** for example, **V** here indicates the return type is **void**; **()** hints the method takes no parameter.

To call a Java method, JNI provides a collection of methods to choose. If the method is static, choose those matching
patter of CallStaticXXXMethodID; If the method returns primitive types, say int, then Call[Static]IntMethodID; If class
 type, then Call[Static]ObjectMethodID.