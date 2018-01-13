#include "gtest/gtest.h"
#include "core.h"

TEST(CoreTest, testInitializeAndShutdown) {
    using namespace io::openmessaging::core;
    JavaOption javaOption(JNI_VERSION_1_8);
    javaOption.addOption("-Xms1G");
    javaOption.addOption("-Xmx1G");
    Initialize(javaOption);
    Shutdown();
}