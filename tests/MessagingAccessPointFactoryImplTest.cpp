#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "MessagingAccessPoint.h"
#include "KeyValueImpl.h"
#include "BaseTest.h"

BEGIN_NAMESPACE_2(io, openmessaging)

    using namespace std;

    class MessagingAccessPointFactoryTest : public BaseTest {

    };

    TEST_F(MessagingAccessPointFactoryTest, testCtor) {

        string accessPointUrl = "oms:rocketmq://localhost:9876/default:default";
        string driverClassKey = "oms.driver.impl";
        string driverClass = "io.openmessaging.rocketmq.MessagingAccessPointImpl";

        KeyValuePtr properties(newKeyValue());
        properties->put(driverClassKey, driverClass);

        MessagingAccessPointPtr messagingAccessPoint(getMessagingAccessPoint(accessPointUrl, properties));

        producer::ProducerPtr producer = messagingAccessPoint->createProducer();
        producer->startup();

        string topic = "TopicTest";
        const char* data = "HELLO";
        scoped_array<char> body(const_cast<char *>(data), strlen(data));
        MessagePtr message = producer->createBytesMessage(topic, body);
        producer::SendResultPtr sendResult = producer->send(message);

        cout << sendResult->messageId() << endl;

        producer->shutdown();
    }

    TEST_F(MessagingAccessPointFactoryTest, testCreatePullConsumer) {

        string accessPointUrl = "oms:rocketmq://localhost:9876/default:default";
        string driverClassKey = "oms.driver.impl";
        string driverClass = "io.openmessaging.rocketmq.MessagingAccessPointImpl";

        KeyValuePtr properties(newKeyValue());
        properties->put(driverClassKey, driverClass);

        MessagingAccessPointPtr messagingAccessPoint(getMessagingAccessPoint(accessPointUrl, properties));
        std::string queueName("TopicTest");


        KeyValuePtr kv(newKeyValue());
        const std::string key = "rmq.consumer.group";
        const std::string value = "OMS_CONSUMER";
        kv->put(key, value);

        consumer::PullConsumerPtr pullConsumer = messagingAccessPoint->createPullConsumer(queueName, kv);

        ASSERT_TRUE(pullConsumer);

        pullConsumer->startup();

        pullConsumer->shutdown();
    }

END_NAMESPACE_2(io, openmessaging)

