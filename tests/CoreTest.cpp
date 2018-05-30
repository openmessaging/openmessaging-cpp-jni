#include <iostream>
#include "gtest/gtest.h"
#include <boost/filesystem.hpp>
#include "core.h"

TEST(CoreTest, testBuildSignature) {
    using namespace io::openmessaging;

    std::string signature = buildSignature(Types::void_, 1, Types::boolean_);
    std::string expectedResult = "(Z)V";
    ASSERT_TRUE(signature == expectedResult);
}