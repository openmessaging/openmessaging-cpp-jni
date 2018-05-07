#include <gtest/gtest.h>
#include "consumer/PullConsumer.h"
#include "MessagingAccessPoint.h"
#include "core.h"
#include "KeyValueImpl.h"
#include "BuiltinKeys.h"
#include "BaseTest.h"

BEGIN_NAMESPACE_2(io, openmessaging)

    using namespace std;

    class PullConsumerImplTest : public BaseTest {
    };

    TEST_F(PullConsumerImplTest, testCreatePullConsumer) {
        string accessPointUrl = "oms:rocketmq://localhost:9876/default:default";

        MessagingAccessPointPtr messagingAccessPoint(getMessagingAccessPointImpl(accessPointUrl));

        // First send a message
        producer::ProducerPtr producer = messagingAccessPoint->createProducer();
        producer->startup();

        string queueName = "TopicTest";
        const char* data = "HELLO";
        MessageBody body(reinterpret_cast<signed char *>(const_cast<char *>(data)), strlen(data));
        MessagePtr message = producer->createBytesMessage(queueName, body);
        producer::SendResultPtr sendResultPtr = producer->send(message);
        LOG_INFO << "Send Message OK. Message Id: " << sendResultPtr->messageId();

        KeyValuePtr kv(newKeyValueImpl());
        const std::string value = "OMS_CONSUMER";
        kv->put(CONSUMER_ID, value);

        consumer::PullConsumerPtr pullConsumer = messagingAccessPoint->createPullConsumer(kv);
        pullConsumer->attachQueue(queueName);

        ASSERT_TRUE(pullConsumer);

        pullConsumer->startup();

        bool stopped = false;
        while (!stopped) {
            MessagePtr msgPtr = pullConsumer->receive();
            if (msgPtr) {

                KeyValuePtr sysHeaders = msgPtr->sysHeaders();
                if (sysHeaders) {
                    std::string msgId = sysHeaders->getString(MESSAGE_ID);
                    LOG_DEBUG << "A message received. MsgId: " << msgId;
                    if (msgId == sendResultPtr->messageId()) {
                        LOG_INFO << "Message which was just sent is received. Stop receiving immediately.";
                        stopped = true;
                    }
                    signed char* data = msgPtr->getBody().get();
                    std::cout << data << std::endl;
                }
            }
        }
        pullConsumer->shutdown();
    }

END_NAMESPACE_2(io, openmessaging)