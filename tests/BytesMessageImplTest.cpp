#include <gtest/gtest.h>
#include "ByteMessageImpl.h"
#include "core.h"

using namespace io::openmessaging;

TEST(BytesMessageImplTest, testCtor) {
    Initialize();
    Shutdown();
}