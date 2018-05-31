#include <gtest/gtest.h>
#include <Singleton.h>
#include <Uncopyable.h>
#include <Namespace.h>
#include <iostream>
#include <plog/Log.h>

BEGIN_NAMESPACE_2(io, openmessaging)

class VirtualMachine : Uncopyable  {
public:
    VirtualMachine() {
        LOG_DEBUG << "VirtualMachine()";
    }

    ~VirtualMachine() {
        LOG_DEBUG << "~VirtualMachine()";
    }

    void f() {
        LOG_DEBUG << "VirtualMachine::f()";
    }
};

END_NAMESPACE_2(io, openmessaging)


TEST(SingletonTest, test_normal) {
    using namespace io::openmessaging;
    VirtualMachine& vm = Singleton<VirtualMachine>::instance();
    vm.f();
}