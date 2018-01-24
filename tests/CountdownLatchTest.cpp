#include <boost/thread.hpp>
#include <gtest/gtest.h>

#include "CountdownLatch.h"

using namespace io::openmessaging;

TEST(CountdownLatchTest, testBasic) {
    CountdownLatch countdownLatch(1);
    countdownLatch.countdown();
    countdownLatch.await();
}

void CountdownLatchTest_f(CountdownLatch &countdownLatch) {
    boost::this_thread::sleep_for(boost::chrono::milliseconds(3000));
    countdownLatch.countdown();
}

TEST(CountdownLatchTest, testSynchronizaiton) {
    CountdownLatch countdownLatch(1);
    boost::thread t(CountdownLatchTest_f, boost::ref(countdownLatch));
    countdownLatch.await();
    if (t.joinable()) {
        t.join();
    }
}

