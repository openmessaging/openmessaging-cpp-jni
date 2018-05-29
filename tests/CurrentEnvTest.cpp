#include <gtest/gtest.h>
#include "core.h"

using namespace io::openmessaging;

class CurrentEnvTest : public ::testing::Test {
public:
    virtual void SetUp() {
    }
};

TEST_F(CurrentEnvTest, testCtor) {
    CurrentEnv *ctx = new CurrentEnv();
    ASSERT_TRUE(ctx->env);
    delete ctx;
}

TEST_F(CurrentEnvTest, testGetClassName) {
    CurrentEnv context;
    jclass klass = context.findClass("java/lang/String");
    std::string name = context.getClassName(klass);
    ASSERT_EQ("java.lang.String", name);

    jclass klassList = context.findClass("java/util/List");
    std::string listName = context.getClassName(klassList);
    ASSERT_EQ("java.util.List", listName);
}
