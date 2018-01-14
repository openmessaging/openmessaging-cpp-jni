#include <iostream>
#include "core.h"
#include <jni.h>

int main() {

    using namespace io::openmessaging::core;

    std::cout << "Hello, World!" << std::endl;

    Initialize();

    Shutdown();

    return 0;
}