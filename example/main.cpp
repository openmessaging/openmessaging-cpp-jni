#include <iostream>
#include "core.h"

int main() {

    using namespace io::openmessaging;

    std::cout << "Hello, World!" << std::endl;

    Initialize();

    Shutdown();

    return 0;
}