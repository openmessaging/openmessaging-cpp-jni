#include <gtest/gtest.h>

#include "consumer/PushConsumerImpl.h"
#include "KeyValueImpl.h"
#include "MessagingAccessPointImpl.h"
#include "NonStandardKeys.h"
#include "CountdownLatch.h"
#include "BaseTest.h"
#include "NonStandardKeys.h"


BEGIN_NAMESPACE_3(io, openmessaging, consumer)

    class ExampleMessageListener : virtual public MessageListener {
    public:
        ExampleMessageListener(CountdownLatch &latch_) : latch(latch_) {
        }

        virtual void onMessage(MessagePtr &message, ContextPtr &context) {
            LOG_DEBUG << "A message received";
            context->ack();
            latch.countdown();
        }

    private:
        CountdownLatch &latch;
    };

END_NAMESPACE_3(io, openmessaging, consumer)

BEGIN_NAMESPACE_2(io, openmessaging)

    using namespace io::openmessaging::consumer;
    using namespace std;

    class PushConsumerImplTest : public BaseTest {

    };


    TEST_F(PushConsumerImplTest, testCreatePushConsumer) {

        string accessPointUrl = "oms:rocketmq://localhost:9876/default:default";

        MessagingAccessPointPtr messagingAccessPoint(getMessagingAccessPointImpl(accessPointUrl));

        // First send a message
        producer::ProducerPtr producer = messagingAccessPoint->createProducer();
        producer->startup();

        string topic = "TopicTest";
        const char *data = "HELLO";
        MessageBody body(reinterpret_cast<signed char *>(const_cast<char *>(data)), strlen(data));
        MessagePtr message = producer->createBytesMessage(topic, body);
        producer->send(message);
        // Send message OK

        std::string queueName("TopicTest");

        KeyValuePtr kv(newKeyValueImpl());
        const std::string value = "OMS_CONSUMER";
        kv->put(CONSUMER_GROUP, value);

        consumer::PushConsumerPtr pushConsumer = messagingAccessPoint->createPushConsumer(kv);

        ASSERT_TRUE(pushConsumer);

        CountdownLatch latch(1);

        MessageListenerPtr messageListener(new ExampleMessageListener(latch));
        pushConsumer->attachQueue(queueName, messageListener);

        pushConsumer->startup();
        LOG_DEBUG << "Push consumer starts OK";
        latch.await();
        pushConsumer->shutdown();
    }

END_NAMESPACE_2(io, openmessaging)

