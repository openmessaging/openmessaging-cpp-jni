#include "gtest/gtest.h"
#include <boost/filesystem.hpp>
#include "core.h"

TEST(CoreTest, testInitializeAndShutdown) {
    using namespace io::openmessaging;
    Initialize();
}

TEST(CoreTest, testStringEndsWith) {
    std::string s("/home/lib/*");
    std::string ext("*");

    ASSERT_TRUE(s.substr(s.size() - ext.size()) == ext);
    ASSERT_TRUE(io::openmessaging::stringEndsWith(s, ext));
}