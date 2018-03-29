#include <gtest/gtest.h>
#include "smart_pointer.h"
#include <iostream>

#if __cplusplus < 201103L

#ifndef USE_BOOST

using namespace io::openmessaging;

class Type {
public:
    Type() {
        std::cout << "Ctor" << std::endl;
    }

    virtual ~Type() {
        std::cout << "Dtor" << std::endl;
    }

    void execute() {
        std::cout << "Dummy function" << std::endl;
    }
};

class Derivative : public Type {

};


TEST(smart_pointer_test, test_ctor_1) {

    Type *t = new Type;
    {
        shared_ptr<Type> ptr(t);
        ptr->execute();
    }

    std::cout << "Should have destructed" << std::endl;
}

TEST(smart_pointer_test, test_ctor_2) {

    Type *t = new Type;
    shared_ptr<Type> p2;
    {
        shared_ptr<Type> ptr(t);

        p2 = ptr;
        p2->execute();
    }
}

TEST(smart_pointer_test, test_make_shared) {
    shared_ptr<Type> ptr = make_shared<Type>();
    ptr->execute();
}

TEST(smart_pointer_test, test_make_shared_2) {
    shared_ptr<int> ptr = make_shared<int>(1);
    std::cout << *ptr << std::endl;
}

TEST(smart_pointer_test, test_dynamic_pointer_cast) {
    shared_ptr<Type> pType = make_shared<Derivative>();
    shared_ptr<Derivative> pDerivative = dynamic_pointer_cast<Derivative>(pType);
    pDerivative->execute();
}
#endif

// pre C++11
#endif