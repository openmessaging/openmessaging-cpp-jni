#include <gtest/gtest.h>
#include "core.h"
#include "CurrentEnv.h"

class CurrentEnvTest : public ::testing::Test {
public:
    virtual void SetUp() {
        io::openmessaging::Initialize();
    }
};

TEST_F(CurrentEnvTest, testCtor) {
    using namespace io::openmessaging;
    CurrentEnv *ctx = new CurrentEnv();
    ASSERT_TRUE(ctx->env);
    delete ctx;
}