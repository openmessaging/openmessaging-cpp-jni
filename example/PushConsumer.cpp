#include <plog/Log.h>
#include "MessagingAccessPointFactory.h"
#include "NonStandardKeys.h"

BEGIN_NAMESPACE_3(io, openmessaging, consumer)

    class ExampleMessageListener : virtual public MessageListener {
    public:

        virtual void onMessage(NS::shared_ptr<Message> &message, NS::shared_ptr<Context> &context) {
            NS::shared_ptr<KeyValue> sysHeaders = message->sysHeaders();
            std::string msgId = sysHeaders->getString(MessageId);
            LOG_INFO << "A new message received. MsgId: " << msgId;
        }
    };

END_NAMESPACE_3(io, openmessaging, consumer)

using namespace std;
using namespace io::openmessaging;
using namespace io::openmessaging::consumer;

int main(int argc, char *argv[]) {

    // Define access point
    const string accessPointUrl = "oms:rocketmq://ons3.dev:9876/default:default";

    // Define implementation driver class key
    const string driverClassKey = "oms.driver.impl";

    // Define implementation driver class
    const string driverClass = "io.openmessaging.rocketmq.MessagingAccessPointImpl";

    load_library(accessPointUrl);

    // Create Key-Value container to hold custom settings
    NS::shared_ptr<KeyValue> kv = NS::shared_ptr<KeyValue>(newKeyValue());

    // Configure driver class
    kv->put(driverClassKey, driverClass);

    // Acquire messaging access point instance through factory method
    NS::shared_ptr<MessagingAccessPoint> accessPoint = NS::shared_ptr<MessagingAccessPoint>(getMessagingAccessPoint(accessPointUrl, kv));

    std::string queueName("TopicTest");

    NS::shared_ptr<KeyValue> subKV = NS::shared_ptr<KeyValue>(newKeyValue());
    const std::string value = "OMS_CONSUMER";
    subKV->put(CONSUMER_GROUP, value);

    NS::shared_ptr<consumer::PushConsumer> pushConsumer = accessPoint->createPushConsumer(subKV);

    // Create a listener instance
    NS::shared_ptr<MessageListener> messageListener = NS::make_shared<ExampleMessageListener>();

    // Attach listener to queue
    pushConsumer->attachQueue(queueName, messageListener);

    // Start push consumer
    pushConsumer->startup();

    LOG_INFO << "Push consumer starts OK";

    sleep(60 * 60);

}