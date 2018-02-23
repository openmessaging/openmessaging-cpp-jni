#include <gtest/gtest.h>

#include "consumer/PushConsumerImpl.h"
#include "KeyValueImpl.h"
#include "MessagingAccessPointImpl.h"
#include "MessagingAccessPointFactory.h"
#include "NonStandardKeys.h"
#include "consumer/MessageListener.h"


BEGIN_NAMESPACE_3(io, openmessaging, consumer)

    class ExampleMessageListener : virtual public MessageListener {
    public:
        virtual void onMessage(boost::shared_ptr<Message>& message, boost::shared_ptr<Context>& context) {
            std::cout << "A message received" << std::endl;
        }
    };

END_NAMESPACE_3(io, openmessaging, consumer)


TEST(PushConsumerImplTest, testCreatePushConsumer) {

    using namespace io::openmessaging;
    using namespace io::openmessaging::consumer;
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

    boost::shared_ptr<consumer::PushConsumer> pushConsumer = messagingAccessPoint->createPushConsumer(kv);

    ASSERT_TRUE(pushConsumer);

    boost::shared_ptr<MessageListener> messageListener = boost::make_shared<ExampleMessageListener>();
    pushConsumer->attachQueue(queueName, messageListener);

    pushConsumer->startup();

    cout << "Push consumer starts OK" << endl;

//    while (true) {
//        boost::shared_ptr<Message> msg = pushConsumer->poll();
//        if (msg) {
//            boost::shared_ptr<KeyValue> sysHeaders = message->sysHeaders();
//            std::string msgId = sysHeaders->getString(BuiltinKeys::MessageId);
//            ASSERT_TRUE(!msgId.empty());
//            ASSERT_NO_THROW(pushConsumer->ack(msgId));
//            break;
//        }
//    }
}