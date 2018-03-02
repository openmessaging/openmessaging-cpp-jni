#include "BaseTest.h"
#include <iostream>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

BEGIN_NAMESPACE_2(io, openmessaging)
    class ServerTest : public BaseTest {

    };

    TEST_F(ServerTest, testNameServer) {
        std::cout << "Name server and broker should have started" << std::endl;
        boost::this_thread::sleep(boost::posix_time::seconds(3));
    }

END_NAMESPACE_2(io, openmessaging)