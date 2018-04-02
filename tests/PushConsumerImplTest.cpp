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

        virtual void onMessage(NS::shared_ptr<Message> &message, NS::shared_ptr<Context> &context) {
            std::cout << "A message received" << std::endl;
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
        string driverClassKey = "oms.driver.impl";
        string driverClass = "io.openmessaging.rocketmq.MessagingAccessPointImpl";

        NS::shared_ptr<KeyValue> properties = NS::make_shared<KeyValueImpl>();
        properties->put(driverClassKey, driverClass);

        NS::shared_ptr<MessagingAccessPoint> messagingAccessPoint =
                MessagingAccessPointFactory::getMessagingAccessPoint(accessPointUrl, properties);

        // First send a message
        NS::shared_ptr<producer::Producer> producer = messagingAccessPoint->createProducer();
        producer->startup();

        string topic = "TopicTest";
        const char *data = "HELLO";
        scoped_array<char> body(const_cast<char *>(data), strlen(data));
        NS::shared_ptr<Message> message = producer->createByteMessageToTopic(topic, body);
        producer->send(message);
        // Send message OK

        std::string queueName("TopicTest");

        NS::shared_ptr<KeyValue> kv = NS::make_shared<KeyValueImpl>();
        const std::string value = "OMS_CONSUMER";
        kv->put(CONSUMER_GROUP, value);

        NS::shared_ptr<consumer::PushConsumer> pushConsumer = messagingAccessPoint->createPushConsumer(kv);

        ASSERT_TRUE(pushConsumer);

        CountdownLatch latch(1);

        NS::shared_ptr<MessageListener> messageListener = NS::make_shared<ExampleMessageListener>(
                NS::ref(latch));
        pushConsumer->attachQueue(queueName, messageListener);

        pushConsumer->startup();
        cout << "Push consumer starts OK" << endl;
        latch.await();
        pushConsumer->shutdown();
    }

END_NAMESPACE_2(io, openmessaging)

