#include <iostream>

#include <gtest/gtest.h>

#include "KeyValueImpl.h"
#include "MessagingAccessPointFactory.h"
#include "producer/ProducerImpl.h"
#include "CountdownLatch.h"
#include "BaseTest.h"

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

    const string ProducerImplTest::accessPointUrl = "oms:rocketmq://ons3.dev:9876/default:default";
    const string ProducerImplTest::driverClassKey = "oms.driver.impl";
    const string ProducerImplTest::driverClass = "io.openmessaging.rocketmq.MessagingAccessPointImpl";


    TEST_F(ProducerImplTest, testSendOneway) {
        boost::shared_ptr<KeyValue> properties = boost::make_shared<KeyValueImpl>();
        properties->put(driverClassKey, driverClass);

        boost::shared_ptr<MessagingAccessPoint> messagingAccessPoint =
                MessagingAccessPointFactory::getMessagingAccessPoint(accessPointUrl, properties);

        boost::shared_ptr<producer::Producer> producer = messagingAccessPoint->createProducer();
        producer->startup();

        string topic = "TopicTest";
        const char* data = "HELLO";
        scoped_array<char> body(const_cast<char *>(data), strlen(data));

        boost::shared_ptr<Message> message = producer->createByteMessageToTopic(topic, body);
        producer->sendOneway(message);
    }

    TEST_F(ProducerImplTest, testSend_accuracy) {
        boost::shared_ptr<KeyValue> properties = boost::make_shared<KeyValueImpl>();
        properties->put(driverClassKey, driverClass);

        boost::shared_ptr<MessagingAccessPoint> messagingAccessPoint =
                MessagingAccessPointFactory::getMessagingAccessPoint(accessPointUrl, properties);

        boost::shared_ptr<producer::Producer> producer = messagingAccessPoint->createProducer();
        producer->startup();

        string topic = "TopicTest";
        const char* data = "HELLO";
        scoped_array<char> body(const_cast<char *>(data), strlen(data));

        boost::shared_ptr<Message> message = producer->createByteMessageToTopic(topic, body);
        for (int i = 0; i < 10000; ++i) {
            producer->send(message);
        }
    }

    TEST_F(ProducerImplTest, testAsynchrousSend) {
        boost::shared_ptr<KeyValue> properties = boost::make_shared<KeyValueImpl>();
        properties->put(driverClassKey, driverClass);

        boost::shared_ptr<MessagingAccessPoint> messagingAccessPoint =
                MessagingAccessPointFactory::getMessagingAccessPoint(accessPointUrl, properties);

        boost::shared_ptr<producer::Producer> producer = messagingAccessPoint->createProducer();
        producer->startup();

        string topic = "TopicTest";
        const char* data = "HELLO";
        scoped_array<char> body(const_cast<char *>(data), strlen(data));

        CountdownLatch latch(1);
        boost::shared_ptr<FutureListener> listener = boost::make_shared<FutureListenerTest>(boost::ref(latch));

        boost::shared_ptr<Message> message = producer->createByteMessageToTopic(topic, body);
        boost::shared_ptr<Future> future = producer->sendAsync(message);
        future->addListener(listener);
        latch.await();
    }
END_NAMESPACE_2(io, openmessaging)

