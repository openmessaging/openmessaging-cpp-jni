#include <iostream>

#include <gtest/gtest.h>

#include "KeyValueImpl.h"
#include "MessagingAccessPointFactory.h"
#include "producer/ProducerImpl.h"
#include "CountdownLatch.h"

BEGIN_NAMESPACE_2(io, openmessaging)
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

END_NAMESPACE_2(io, openmessaging)

TEST(ProducerImplTest, testSendOneway) {
    using namespace io::openmessaging;

    using namespace io::openmessaging;
    using namespace std;

    string accessPointUrl = "oms:rocketmq://localhost:9876/default:default";
    string driverClassKey = "oms.driver.impl";
    string driverClass = "io.openmessaging.rocketmq.MessagingAccessPointImpl";

    Initialize();

    boost::shared_ptr<KeyValue> properties = boost::make_shared<KeyValueImpl>();
    properties->put(driverClassKey, driverClass);

    boost::shared_ptr<MessagingAccessPoint> messagingAccessPoint =
            MessagingAccessPointFactory::getMessagingAccessPoint(accessPointUrl, properties);

    boost::shared_ptr<producer::Producer> producer = messagingAccessPoint->createProducer();
    producer->startup();

    string topic = "TopicTest";
    vector<unsigned char> body;
    body.push_back('H');
    body.push_back('E');
    body.push_back('L');
    body.push_back('L');
    body.push_back('O');

    boost::shared_ptr<Message> message = producer->createByteMessageToTopic(topic, body);
    producer->sendOneway(message);
}

TEST(ProducerImplTest, testSend_accuracy) {
    using namespace io::openmessaging;

    using namespace io::openmessaging;
    using namespace std;

    string accessPointUrl = "oms:rocketmq://localhost:9876/default:default";
    string driverClassKey = "oms.driver.impl";
    string driverClass = "io.openmessaging.rocketmq.MessagingAccessPointImpl";

    Initialize();

    boost::shared_ptr<KeyValue> properties = boost::make_shared<KeyValueImpl>();
    properties->put(driverClassKey, driverClass);

    boost::shared_ptr<MessagingAccessPoint> messagingAccessPoint =
            MessagingAccessPointFactory::getMessagingAccessPoint(accessPointUrl, properties);

    boost::shared_ptr<producer::Producer> producer = messagingAccessPoint->createProducer();
    producer->startup();

    string topic = "TopicTest";
    vector<unsigned char> body;
    body.push_back('H');
    body.push_back('E');
    body.push_back('L');
    body.push_back('L');
    body.push_back('O');

    boost::shared_ptr<Message> message = producer->createByteMessageToTopic(topic, body);
    for (int i = 0; i < 10000; ++i) {
        producer->send(message);
    }
}

TEST(ProducerImplTest, testAsynchrousSend) {
    using namespace io::openmessaging;
    using namespace std;

    string accessPointUrl = "oms:rocketmq://localhost:9876/default:default";
    string driverClassKey = "oms.driver.impl";
    string driverClass = "io.openmessaging.rocketmq.MessagingAccessPointImpl";

    Initialize();

    boost::shared_ptr<KeyValue> properties = boost::make_shared<KeyValueImpl>();
    properties->put(driverClassKey, driverClass);

    boost::shared_ptr<MessagingAccessPoint> messagingAccessPoint =
            MessagingAccessPointFactory::getMessagingAccessPoint(accessPointUrl, properties);

    boost::shared_ptr<producer::Producer> producer = messagingAccessPoint->createProducer();
    producer->startup();

    string topic = "TopicTest";
    vector<unsigned char> body;
    body.push_back('H');
    body.push_back('E');
    body.push_back('L');
    body.push_back('L');
    body.push_back('O');

    CountdownLatch latch(1);
    boost::shared_ptr<FutureListener> listener = boost::make_shared<FutureListenerTest>(boost::ref(latch));

    boost::shared_ptr<Message> message = producer->createByteMessageToTopic(topic, body);
    boost::shared_ptr<Future> future = producer->sendAsync(message);
    future->addListener(listener);
    latch.await();
}