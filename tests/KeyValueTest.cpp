#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <gtest/gtest.h>


#include "KeyValue.h"


TEST(KeyValueTest, testPut) {
    using namespace io::openmessaging;
    KeyValue kv;
    kv.put("a", 1).put("b", 2L).put("c", 3.0F).put("d", 4.0);
}

TEST(KeyValueTest, testGet) {
    using namespace io::openmessaging;
    KeyValue kv;
    kv.put("a", 1).put("b", 2L).put("c", 3.0F).put("d", 4.0);

    int a = kv.get<int>("a");
    ASSERT_EQ(1, a);

    long b = kv.get<long>("b");
    ASSERT_EQ(2L, b);

    float c = kv.get<float>("c");
    ASSERT_FLOAT_EQ(3.0F, c);

    double d = kv.get<double>("d");
    ASSERT_DOUBLE_EQ(4.0, d);
}

TEST(KeyValueTest, testSharedPtr) {
    using namespace io::openmessaging;
    boost::shared_ptr<KeyValue> kv_nullptr;

    boost::shared_ptr<KeyValue> kv(boost::make_shared<KeyValue>());

}