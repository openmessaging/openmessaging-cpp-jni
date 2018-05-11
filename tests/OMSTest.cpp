#include <string>

#include <gtest/gtest.h>

#include "core.h"
#include "OMS.h"
#include "KeyValueImpl.h"
#include "BaseTest.h"
#include "MessagingAccessPoint.h"

BEGIN_NAMESPACE_2(io, openmessaging)

    using namespace std;

    class OmsTest : public BaseTest {

    };

    TEST_F(OmsTest, testCtor) {

        string accessPointUrl = "oms:rocketmq://localhost:9876/default:default";

        MessagingAccessPointPtr messagingAccessPoint = getMessagingAccessPointImpl(accessPointUrl);

        producer::ProducerPtr producer = messagingAccessPoint->createProducer();
        producer->startup();

        string topic = "TopicTest";
        const char* data = "HELLO";
        MessageBody body(reinterpret_cast<signed char*>(const_cast<char *>(data)), strlen(data));
        MessagePtr message = producer->createBytesMessage(topic, body);
        producer::SendResultPtr sendResult = producer->send(message);

        cout << sendResult->messageId() << endl;

        producer->shutdown();
    }

    TEST_F(OmsTest, testCreatePullConsumer) {

        string accessPointUrl = "oms:rocketmq://localhost:9876/default:default";

        MessagingAccessPointPtr messagingAccessPoint = getMessagingAccessPointImpl(accessPointUrl);

        KeyValuePtr kv = newKeyValueImpl();
        const std::string key = "consumer.id";
        const std::string value = "OMS_CONSUMER";
        kv->put(key, value);

        consumer::PullConsumerPtr pullConsumer = messagingAccessPoint->createPullConsumer(kv);

        std::string queueName("TopicTest");
        pullConsumer->attachQueue(queueName);

        ASSERT_TRUE(pullConsumer);

        pullConsumer->startup();

        pullConsumer->shutdown();
    }

    TEST_F(OmsTest, testNewKeyValue) {
        using namespace io::openmessaging;

        KeyValuePtr kv = newKeyValueImpl();
        ASSERT_TRUE(kv);

        jobject attr = (dynamic_cast<KeyValueImpl*>(kv.get()))->getProxy();
        ASSERT_TRUE(NULL != attr);
    }
END_NAMESPACE_2(io, openmessaging)