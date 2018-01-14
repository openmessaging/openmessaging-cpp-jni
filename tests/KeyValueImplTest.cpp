#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <gtest/gtest.h>
#include "KeyValueImpl.h"


TEST(KeyValueImplTest, testPut) {
    using namespace io::openmessaging;
    boost::shared_ptr<KeyValue> kvPtr = boost::make_shared<core::KeyValueImpl>();
}
