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
    char *rocketmqHome = getenv("ROCKETMQ_HOME");
    if (NULL == rocketmqHome) {
        LOG_ERROR << "Environment variable ROCKETMQ_HOME is not set";
        FAIL();
    }
    std::string path = std::string(rocketmqHome) + "/lib";
    std::vector<std::string> files = io::openmessaging::list_files(path, io::openmessaging::file_name_filter);
    for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); it++) {
        std::cout << *it << std::endl;
    }
}