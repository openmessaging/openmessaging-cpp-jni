#include <gtest/gtest.h>

#include "core.h"
#include "OMS.h"
#include "KeyValueImpl.h"

class OMSTest : public ::testing::Test {
public:
    virtual void SetUp() {
        io::openmessaging::Initialize();
    }
};

TEST_F(OMSTest, testNewKeyValue) {
    using namespace io::openmessaging;

    boost::shared_ptr<KeyValue> kv = OMS::newKeyValue();
    ASSERT_TRUE(kv);

    boost::shared_ptr<KeyValueImpl> impl = boost::dynamic_pointer_cast<KeyValueImpl>(kv);
    ASSERT_TRUE(impl);

    jobject  proxy = impl->getProxy();
    ASSERT_TRUE(proxy);
}