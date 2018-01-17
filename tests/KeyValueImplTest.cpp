#include <set>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <gtest/gtest.h>
#include "KeyValueImpl.h"


TEST(KeyValueImplTest, testPutGetInt) {
    using namespace io::openmessaging;
    boost::shared_ptr<KeyValue> kvPtr = boost::make_shared<core::KeyValueImpl>();
    std::string k = "a";
    int value = 1;
    kvPtr->put(k, value);

    ASSERT_TRUE(kvPtr->containsKey(k));
    ASSERT_EQ(value, kvPtr->getInt(k));
}

TEST(KeyValueImplTest, testPutGetLong) {
    using namespace io::openmessaging;
    boost::shared_ptr<KeyValue> kvPtr = boost::make_shared<core::KeyValueImpl>();
    std::string k = "a";
    long value = 1;
    kvPtr->put(k, value);

    ASSERT_TRUE(kvPtr->containsKey(k));
    ASSERT_EQ(value, kvPtr->getLong(k));
}

TEST(KeyValueImplTest, testPutGetDouble) {
    using namespace io::openmessaging;
    boost::shared_ptr<KeyValue> kvPtr = boost::make_shared<core::KeyValueImpl>();
    std::string k = "a";
    double value = 1.0;
    kvPtr->put(k, value);

    ASSERT_TRUE(kvPtr->containsKey(k));
    ASSERT_DOUBLE_EQ(value, kvPtr->getDouble(k));
}

TEST(KeyValueImplTest, testKeySet) {
    using namespace io::openmessaging;

    boost::shared_ptr<KeyValue> kvPtr = boost::make_shared<core::KeyValueImpl>();

    std::string k1 = "a";
    int v1 = 1;

    std::string k2 = "b";
    long v2 = 2;

    kvPtr->put(k1, v1);
    kvPtr->put(k2, v2);

    std::set<std::string> keys = kvPtr->keySet();
    ASSERT_EQ(2, keys.size());

    for(std::set<std::string>::const_iterator it = keys.cbegin(); it != keys.cend(); it++) {
        std::cout << *it << std::endl;
    }
}
