#include <gtest/gtest.h>

#include "core.h"
#include "OMS.h"
#include "KeyValueImpl.h"

BEGIN_NAMESPACE_2(io, openmessaging)
    class OMSTest : public ::testing::Test {
    public:
        virtual void SetUp() {
            io::openmessaging::Initialize();
        }
    };

    TEST_F(OMSTest, testNewKeyValue) {
        using namespace io::openmessaging;

        KeyValuePtr kv(newKeyValueImpl());
        ASSERT_TRUE(kv);

        KeyValueImplPtr impl = NS::dynamic_pointer_cast<KeyValueImpl>(kv);
        ASSERT_TRUE(impl);

        jobject  proxy = impl->getProxy();
        ASSERT_TRUE(proxy);
    }
END_NAMESPACE_2(io, openmessaging)