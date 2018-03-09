#include <gtest/gtest.h>
#include "consumer/PullConsumer.h"
#include "MessagingAccessPoint.h"
#include "MessagingAccessPointFactory.h"
#include "core.h"
#include "KeyValueImpl.h"
#include "NonStandardKeys.h"
#include "BaseTest.h"

BEGIN_NAMESPACE_2(io, openmessaging)

    using namespace std;

    class PullConsumerImplTest : public BaseTest {
    };

    TEST_F(PullConsumerImplTest, testCreatePullConsumer) {
        string accessPointUrl = "oms:rocketmq://localhost:9876/default:default";
        string driverClassKey = "oms.driver.impl";
        string driverClass = "io.openmessaging.rocketmq.MessagingAccessPointImpl";

        boost::shared_ptr<KeyValue> properties = boost::make_shared<KeyValueImpl>();
        properties->put(driverClassKey, driverClass);

        boost::shared_ptr<MessagingAccessPoint> messagingAccessPoint =
                MessagingAccessPointFactory::getMessagingAccessPoint(accessPointUrl, properties);

        // First send a message
        boost::shared_ptr<producer::Producer> producer = messagingAccessPoint->createProducer();
        producer->startup();

        string topic = "TopicTest";
        const char* data = "HELLO";
        scoped_array<char> body(const_cast<char *>(data), strlen(data));
        boost::shared_ptr<Message> message = producer->createByteMessageToTopic(topic, body);
        producer->send(message);
        // Send message OK

        std::string queueName("TopicTest");

        boost::shared_ptr<KeyValue> kv = boost::make_shared<KeyValueImpl>();
        const std::string value = "OMS_CONSUMER";
        kv->put(CONSUMER_GROUP, value);

        boost::shared_ptr<consumer::PullConsumer> pullConsumer = messagingAccessPoint->createPullConsumer(queueName, kv);

        ASSERT_TRUE(pullConsumer);

        pullConsumer->startup();

        while (true) {
            boost::shared_ptr<Message> msg = pullConsumer->poll();
            if (msg) {
                boost::shared_ptr<KeyValue> sysHeaders = message->sysHeaders();
                std::string msgId = sysHeaders->getString(MessageId);
                ASSERT_TRUE(!msgId.empty());
                ASSERT_NO_THROW(pullConsumer->ack(msgId));
                break;
            }
        }
    }

END_NAMESPACE_2(io, openmessaging)