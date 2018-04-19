#include "BaseTest.h"

BEGIN_NAMESPACE_2(io, openmessaging)
    class ServerTest : public BaseTest {

    };

    TEST_F(ServerTest, testNameServer) {
        LOG_DEBUG << "Name server and broker should have started";
    }

END_NAMESPACE_2(io, openmessaging)