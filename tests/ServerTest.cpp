#include "BaseTest.h"

BEGIN_NAMESPACE_2(io, openmessaging)
    class ServerTest : public BaseTest {

    };

    TEST_F(ServerTest, testNameServer) {
        std::cout << "Name server and broker should have started" << std::endl;
    }

END_NAMESPACE_2(io, openmessaging)