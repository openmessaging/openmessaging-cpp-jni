## Export C++ Assets to C

1. Define a C compatible header

    ```C
    ﻿#ifndef TEST_H
    #define TEST_H
    
    #ifdef __cplusplus
    class Test {
    public:
        void f();
    };
    #endif
    
    #ifdef __cplusplus
    extern "C" {
    #endif
        // Define an opaque type
        struct OmsEnv;
    
        struct OmsEnv * create();
    
        void f(struct OmsEnv *t);
    
    #ifdef __cplusplus
    }
    #endif
    
    #endif // TEST_H
    ```
1. Create an implementation

    ```
    ﻿#include <iostream>
    
    #include "test.h"
    
    void Test::f() {
        std::cout << "test.cpp" << std::endl;
    }
    
    struct OmsEnv * create() {
        struct OmsEnv * t = reinterpret_cast<struct OmsEnv*>(new Test);
        return t;
    }
    
    void f(OmsEnv *t) {
        Test *pt = reinterpret_cast<Test*>(t);
        pt->f();
    }
    
    ```
    
1. Create an example executable

    ```
    ﻿#include <stdio.h>
    
    #include "test.h"
    
    int main(int argc, char **argv) {
        struct OmsEnv *p = create();
        f(p);
    }
    ```
    
    