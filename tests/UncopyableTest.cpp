#include <gtest/gtest.h>

#include "Uncopyable.h"

using namespace io::openmessaging;

BEGIN_NAMESPACE_2(io, openmessaging)

class UncopyableTest : private Uncopyable {

};

END_NAMESPACE_2(io, openmessaging)


TEST(UncopyableTest, TestCopyCtor) {
    UncopyableTest test;
    UncopyableTest test2;

    // The following will cause compilation failure.
    // UncopyableTest test2(test);
    // test2 = test;
}