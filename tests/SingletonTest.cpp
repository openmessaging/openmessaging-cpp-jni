#include <gtest/gtest.h>
#include <Singleton.h>
#include <Uncopyable.h>
#include <Namespace.h>
#include <iostream>

BEGIN_NAMESPACE_2(io, openmessaging)

class VirtualMachine : Uncopyable  {
public:
    VirtualMachine() {
        std::cout << "Ctor" << std::endl;
    }

    ~VirtualMachine() {
        std::cout << "~Ctor" << std::endl;
    }

    void f() {
        std::cout << "f()" << std::endl;
    }
};

END_NAMESPACE_2(io, openmessaging)


TEST(SingletonTest, test_normal) {
    using namespace io::openmessaging;
    VirtualMachine& vm = Singleton<VirtualMachine>::instance();
    vm.f();
}