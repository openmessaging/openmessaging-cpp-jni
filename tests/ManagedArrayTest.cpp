#include <iostream>
#include <gtest/gtest.h>
#include "OMS.h"

using namespace std;
using namespace io::openmessaging;

TEST(ManagedArrayTest, testCtor) {
    const int len = 128;
    char *data = new char[len];
    ManagedArray<char> array(data, len);
    ManagedArray<char> array2 = array;
    array[0] = 'a';
    ASSERT_FALSE(array[0] == array2[0]);
    ASSERT_FALSE(array.get() == array2.get());
    ASSERT_EQ(array.length(), array2.length());
}