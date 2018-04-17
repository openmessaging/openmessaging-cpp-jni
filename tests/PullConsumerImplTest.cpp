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

        KeyValuePtr properties(newKeyValueImpl());
        properties->put(driverClassKey, driverClass);

        MessagingAccessPointPtr messagingAccessPoint(getMessagingAccessPointImpl(accessPointUrl, properties));

        // First send a message
        producer::ProducerPtr producer = messagingAccessPoint->createProducer();
        producer->startup();

        string topic = "TopicTest";
        const char* data = "HELLO";
        MessageBody body(reinterpret_cast<signed char *>(const_cast<char *>(data)), strlen(data));
        MessagePtr message = producer->createBytesMessage(topic, body);
        producer->send(message);
        // Send message OK

        std::string queueName("TopicTest");

        KeyValuePtr kv(newKeyValueImpl());
        const std::string value = "OMS_CONSUMER";
        kv->put(CONSUMER_GROUP, value);

        consumer::PullConsumerPtr pullConsumer = messagingAccessPoint->createPullConsumer(queueName, kv);

        ASSERT_TRUE(pullConsumer);

        pullConsumer->startup();

        while (true) {
            MessagePtr msg = pullConsumer->receive();
            if (msg) {
                KeyValuePtr sysHeaders = message->sysHeaders();
                std::string msgId = sysHeaders->getString(MessageId);
                ASSERT_TRUE(!msgId.empty());
                ASSERT_NO_THROW(pullConsumer->ack(msgId));
                break;
            }
        }
    }

END_NAMESPACE_2(io, openmessaging)