## Dynamic Linking

```
﻿#include <iostream>

#include <dlfcn.h>

#include "test.h"

int main()
{
    struct OmsEnv* (*create)();
    void           (*f)(struct OmsEnv*);


    void* fd = dlopen("/Users/lizhanhui/QtProjects/CppTutorial/build/libtest1.dylib", RTLD_LAZY | RTLD_GLOBAL);

    if (!fd) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    create = (struct OmsEnv* (*)()) dlsym(fd, "create");
    f = (void (*)(struct OmsEnv*)) dlsym(fd, "f");

    std::cout << "It works" << std::endl;

    struct OmsEnv *env = create();
    f(env);

    dlclose(fd);
    return 0;
}

```