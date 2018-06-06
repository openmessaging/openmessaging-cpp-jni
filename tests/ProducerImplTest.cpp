#include <iostream>

#include <gtest/gtest.h>

#include "KeyValueImpl.h"
#include "producer/ProducerImpl.h"
#include "CountdownLatch.h"
#include "BaseTest.h"
#include "MessagingAccessPoint.h"
#include "OMS.h"

BEGIN_NAMESPACE_2(io, openmessaging)

    using namespace io::openmessaging;
    using namespace std;

    class FutureListenerTest : public FutureListener {
    public:
        FutureListenerTest(CountdownLatch &latch) : _latch(latch) {

        }

        virtual void operationComplete(const Future& future);

    private:
        CountdownLatch &_latch;
    };

    void FutureListenerTest::operationComplete(const Future &future) {
        std::cout << "Operation completed" << std::endl;
        _latch.countdown();
    }

    class ProducerImplTest : public BaseTest {

    protected:
        const static string accessPointUrl;
        const static string driverClassKey;
        const static string driverClass;
    };

    const string ProducerImplTest::accessPointUrl = "oms:rocketmq://localhost:9876/default:default";
    const string ProducerImplTest::driverClassKey = "oms.driver.impl";
    const string ProducerImplTest::driverClass = "io.openmessaging.rocketmq.MessagingAccessPointImpl";

    TEST_F(ProducerImplTest, testSend_accuracy) {
        KeyValuePtr properties = newKeyValueImpl();
        properties->put(driverClassKey, driverClass);

        MessagingAccessPointPtr messagingAccessPoint = getMessagingAccessPointImpl(accessPointUrl, properties);

        producer::ProducerPtr producer = messagingAccessPoint->createProducer();
        producer->startup();

        string topic = "TopicTest";
        const char* data = "HELLO";
        MessageBody body(reinterpret_cast<signed char *>(const_cast<char *>(data)), strlen(data));

        MessagePtr message = producer->createBytesMessage(topic, body);
        for (int i = 0; i < 1024; ++i) {
            producer::SendResultPtr sendResultPtr = producer->send(message);
            LOG_DEBUG << "Send OK. Message Id: " << sendResultPtr->messageId();
        }

        producer->shutdown();
    }

    TEST_F(ProducerImplTest, testAsynchrousSend) {
        KeyValuePtr properties = newKeyValueImpl();
        properties->put(driverClassKey, driverClass);

        MessagingAccessPointPtr messagingAccessPoint = getMessagingAccessPointImpl(accessPointUrl, properties);

        producer::ProducerPtr producer = messagingAccessPoint->createProducer();
        producer->startup();

        string topic = "TopicTest";
        const char* data = "HELLO";
        MessageBody body(reinterpret_cast<signed char *>(const_cast<char *>(data)), strlen(data));

        CountdownLatch latch(1);
        FutureListenerPtr listener(new FutureListenerTest(latch));

        MessagePtr message = producer->createBytesMessage(topic, body);
        FuturePtr future = producer->sendAsync(message);
        future->addListener(listener);
        latch.await();

        producer->shutdown();
    }
END_NAMESPACE_2(io, openmessaging)

