#include <plog/Log.h>
#include <OpenMessaging.h>

BEGIN_NAMESPACE_3(io, openmessaging, consumer)

    class ExampleMessageListener : virtual public MessageListener {
    public:

        virtual void onMessage(MessagePtr &message, ContextPtr &context) {
            KeyValuePtr sysHeaders = message->sysHeaders();
            std::string msgId = sysHeaders->getString(MESSAGE_ID);
            LOG_INFO << "A new message received. MsgId: " << msgId;
            context->ack();
        }
    };

END_NAMESPACE_3(io, openmessaging, consumer)

using namespace std;
using namespace io::openmessaging;
using namespace io::openmessaging::consumer;

int main(int argc, char *argv[]) {

    // Define access point
    const string accessPointUrl = "oms:rocketmq://ons3.dev:9876/default:default";

    // Acquire messaging access point instance through factory method
    MessagingAccessPointPtr accessPoint(getMessagingAccessPoint(accessPointUrl));

    std::string queueName("TopicTest");

    KeyValuePtr subKV(newKeyValue());
    const std::string value = "OMS_CONSUMER";
    subKV->put(CONSUMER_ID, value);

    consumer::PushConsumerPtr pushConsumer = accessPoint->createPushConsumer(subKV);

    // Create a listener instance
    MessageListenerPtr messageListener(new ExampleMessageListener);

    // Attach listener to queue
    pushConsumer->attachQueue(queueName, messageListener);

    // Start push consumer
    pushConsumer->startup();

    LOG_INFO << "Push consumer starts OK";

    sleep(60 * 60);

}