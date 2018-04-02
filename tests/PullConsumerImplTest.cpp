#include <gtest/gtest.h>
#include "consumer/PullConsumer.h"
#include "MessagingAccessPoint.h"
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

        NS::shared_ptr<KeyValue> properties = NS::make_shared<KeyValueImpl>();
        properties->put(driverClassKey, driverClass);

        NS::shared_ptr<MessagingAccessPoint> messagingAccessPoint(getMessagingAccessPoint(accessPointUrl, properties));

        // First send a message
        NS::shared_ptr<producer::Producer> producer = messagingAccessPoint->createProducer();
        producer->startup();

        string topic = "TopicTest";
        const char* data = "HELLO";
        scoped_array<char> body(const_cast<char *>(data), strlen(data));
        NS::shared_ptr<Message> message = producer->createByteMessageToTopic(topic, body);
        producer->send(message);
        // Send message OK

        std::string queueName("TopicTest");

        NS::shared_ptr<KeyValue> kv = NS::make_shared<KeyValueImpl>();
        const std::string value = "OMS_CONSUMER";
        kv->put(CONSUMER_GROUP, value);

        NS::shared_ptr<consumer::PullConsumer> pullConsumer = messagingAccessPoint->createPullConsumer(queueName, kv);

        ASSERT_TRUE(pullConsumer);

        pullConsumer->startup();

        while (true) {
            NS::shared_ptr<Message> msg = pullConsumer->poll();
            if (msg) {
                NS::shared_ptr<KeyValue> sysHeaders = message->sysHeaders();
                std::string msgId = sysHeaders->getString(MessageId);
                ASSERT_TRUE(!msgId.empty());
                ASSERT_NO_THROW(pullConsumer->ack(msgId));
                break;
            }
        }
    }

END_NAMESPACE_2(io, openmessaging)