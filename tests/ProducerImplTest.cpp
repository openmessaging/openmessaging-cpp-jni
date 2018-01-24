#include <gtest/gtest.h>

#include "KeyValueImpl.h"
#include "MessagingAccessPointFactory.h"
#include "producer/ProducerImpl.h"

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