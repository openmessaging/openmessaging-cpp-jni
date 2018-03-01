#include <iostream>
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

TEST(CoreTest, testList) {
    std::string path = "/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib";
    std::vector<std::string> files = io::openmessaging::list(path, io::openmessaging::file_name_filter);
    for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); it++) {
        std::cout << *it << std::endl;
    }
}