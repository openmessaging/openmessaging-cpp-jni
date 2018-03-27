#include <set>
#include <string>

#include <gtest/gtest.h>
#include "core.h"
#include "KeyValueImpl.h"

class KeyValueImplTest : public ::testing::Test {
public:
    virtual void SetUp() {
        io::openmessaging::Initialize();
    }
};

TEST_F(KeyValueImplTest, testPutGetInt) {
    using namespace io::openmessaging;
    NS::shared_ptr<KeyValue> kvPtr = NS::make_shared<KeyValueImpl>();
    std::string k = "a";
    int value = 1;
    kvPtr->put(k, value);

    ASSERT_TRUE(kvPtr->containsKey(k));
    ASSERT_EQ(value, kvPtr->getInt(k));
}

TEST_F(KeyValueImplTest, testPutGetLong) {
    using namespace io::openmessaging;
    NS::shared_ptr<KeyValue> kvPtr = NS::make_shared<KeyValueImpl>();
    std::string k = "a";
    long value = 1;
    kvPtr->put(k, value);

    ASSERT_TRUE(kvPtr->containsKey(k));
    ASSERT_EQ(value, kvPtr->getLong(k));
}

TEST_F(KeyValueImplTest, testPutGetDouble) {
    using namespace io::openmessaging;
    NS::shared_ptr<KeyValue> kvPtr = NS::make_shared<KeyValueImpl>();
    std::string k = "a";
    double value = 1.0;
    kvPtr->put(k, value);

    ASSERT_TRUE(kvPtr->containsKey(k));
    ASSERT_DOUBLE_EQ(value, kvPtr->getDouble(k));
}

TEST_F(KeyValueImplTest, testKeySet) {
    using namespace io::openmessaging;

    NS::shared_ptr<KeyValue> kvPtr = NS::make_shared<KeyValueImpl>();

    std::string k1 = "a";
    int v1 = 1;

    std::string k2 = "b";
    long v2 = 2;

    kvPtr->put(k1, v1);
    kvPtr->put(k2, v2);

    std::set<std::string> keys = kvPtr->keySet();
    ASSERT_EQ(2, keys.size());

    for(std::set<std::string>::iterator it = keys.begin(); it != keys.end(); it++) {
        std::cout << *it << std::endl;
    }
}
