#include <iostream>
#include <gtest/gtest.h>
#include "OMS.h"

using namespace std;
using namespace io::openmessaging;

TEST(scoped_array_test, test_ctor) {
    const int len = 128;
    char *data = new char[len];
    scoped_array<char> array(data, len);
    scoped_array<char> array2 = array;
    array[0] = 'a';
    ASSERT_FALSE(array[0] == array2[0]);
    ASSERT_FALSE(array.getRawPtr() == array2.getRawPtr());
}