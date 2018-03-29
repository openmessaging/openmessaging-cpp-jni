#include <gtest/gtest.h>
#include "smart_pointer.h"
#include <iostream>
#include <map>

#if __cplusplus < 201103L

#ifndef USE_BOOST

using namespace io::openmessaging;

class Type {
public:
    Type() {
        std::cout << "Ctor" << std::endl;
    }

    Type(const Type& other) {
        std::cout << "Copy Ctor" << std::endl;
    }

    Type& operator=(const Type &other) {
        std::cout << "Assignment Operator" << std::endl;
        return *this;
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

TEST(smart_pointer_test, test_work_with_map_0) {
    const int COUNT = 2;
    std::map<int, Type> m;
    for (int i = 0; i < COUNT; ++i) {
        m[i];
        m[i] = Type();
        std::cout << "---------------------------" << std::endl;
    }

    for (int j = 0; j < COUNT; ++j) {
        m[j].execute();
    }
}

TEST(smart_pointer_test, test_work_with_map_1) {
    const int COUNT = 2;
    std::map<int, shared_ptr<Type> > m;
    for (int i = 0; i < COUNT; ++i) {
        m[i] = shared_ptr<Type>(new Type());
    }

    for (int j = 0; j < COUNT; ++j) {
        m[j]->execute();
    }
}

TEST(smart_pointer_test, test_work_with_map_2) {
    const int COUNT = 2;
    std::map<int, shared_ptr<Type> > m;
    for (int i = 0; i < COUNT; ++i) {
        m[i] = make_shared<Type>();
    }

    for (int j = 0; j < COUNT; ++j) {
        m[j]->execute();
    }
}

TEST(smart_pointer_test, test_work_with_map_3) {
    const int COUNT = 2;
    std::map<int, shared_ptr<Type> > m;
    for (int i = 0; i < COUNT; ++i) {
        m[i] = make_shared<Derivative>();
    }

    for (int j = 0; j < COUNT; ++j) {
        m[j]->execute();
    }
}

// pre C++11
#endif