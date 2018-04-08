#include <plog/Log.h>
#include "NonStandardKeys.h"
#include "OMS.h"
#include "consumer/MessageListener.h"
#include "consumer/PushConsumer.h"
#include "MessagingAccessPoint.h"

BEGIN_NAMESPACE_3(io, openmessaging, consumer)

    class ExampleMessageListener : virtual public MessageListener {
    public:

        virtual void onMessage(MessagePtr &message, ContextPtr &context) {
            KeyValuePtr sysHeaders = message->sysHeaders();
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
    KeyValuePtr kv(newKeyValue());

    // Configure driver class
    kv->put(driverClassKey, driverClass);

    // Acquire messaging access point instance through factory method
    MessagingAccessPointPtr accessPoint(getMessagingAccessPoint(accessPointUrl, kv));

    std::string queueName("TopicTest");

    KeyValuePtr subKV(newKeyValue());
    const std::string value = "OMS_CONSUMER";
    subKV->put(CONSUMER_GROUP, value);

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