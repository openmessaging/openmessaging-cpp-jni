#include "gtest/gtest.h"
#include "core.h"

TEST(CoreTest, testInitializeAndShutdown) {
    using namespace io::openmessaging;
    Initialize();
}
