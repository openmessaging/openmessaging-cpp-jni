#include <iostream>
#include "gtest/gtest.h"
#include "core.h"
#include "CountdownLatch.h"

using namespace io::openmessaging;

TEST(CoreTest, testBuildSignature) {
    std::string signature = buildSignature(Types::void_, 1, Types::boolean_);
    std::string expectedResult = "(Z)V";
    ASSERT_TRUE(signature == expectedResult);
}

void* doCountDown(void* latch) {
    if (latch) {
        static_cast<CountdownLatch*>(latch)->countdown();
    }
    std::cout << "Count down by 1" << std::endl;
    return NULL;
}

TEST(CountdownLatchTest, testCountdownLatch) {
    int count = 1;
    CountdownLatch countdown(count);
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_t threads[count];
    for (int i = 0; i < count; ++i) {
        pthread_create(&threads[i], &attr, doCountDown, &countdown);
    }
    pthread_attr_destroy(&attr);

    std::cout << "Start to await" << std::endl;
    countdown.await();
    std::cout << "Await completes" << std::endl;

    void* status;
    for (int j = 0; j < count; ++j) {
        int rc = pthread_join(threads[j], &status);
        ASSERT_EQ(rc, 0);
    }
}