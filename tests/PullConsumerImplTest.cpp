#include <gtest/gtest.h>
#include "consumer/PullConsumer.h"
#include "MessagingAccessPoint.h"
#include "MessagingAccessPointFactory.h"
#include "core.h"
#include "KeyValueImpl.h"
#include "NonStandardKeys.h"

TEST(PullConsumerImplTest, testCreatePullConsumer) {

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

    // First send a message
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
    producer->send(message);
    // Send message OK

    std::string queueName("TopicTest");

    boost::shared_ptr<KeyValue> kv = boost::make_shared<KeyValueImpl>();
    const std::string value = "OMS_CONSUMER";
    kv->put(NonStandardKeys::CONSUMER_GROUP, value);

    boost::shared_ptr<consumer::PullConsumer> pullConsumer = messagingAccessPoint->createPullConsumer(queueName, kv);

    ASSERT_TRUE(pullConsumer);

    pullConsumer->startup();

    while (true) {
        boost::shared_ptr<Message> msg = pullConsumer->poll();
        if (msg) {
            boost::shared_ptr<KeyValue> sysHeaders = message->sysHeaders();
            std::string msgId = sysHeaders->getString(BuiltinKeys::MessageId);
            ASSERT_TRUE(!msgId.empty());
            ASSERT_NO_THROW(pullConsumer->ack(msgId));
            break;
        }
    }
}