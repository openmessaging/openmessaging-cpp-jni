#include <boost/smart_ptr.hpp>

#include <plog/Log.h>

#include <MessagingAccessPointFactory.h>
#include "core.h"
#include "NonStandardKeys.h"

BEGIN_NAMESPACE_3(io, openmessaging, consumer)

    class ExampleMessageListener : virtual public MessageListener {
    public:

        virtual void onMessage(boost::shared_ptr<Message> &message, boost::shared_ptr<Context> &context) {
            boost::shared_ptr<KeyValue> sysHeaders = message->sysHeaders();
            std::string msgId = sysHeaders->getString(BuiltinKeys::MessageId);
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

    // Initialize SDK
    Initialize();

    // Create Key-Value container to hold custom settings
    boost::shared_ptr<KeyValue> kv = OMS::newKeyValue();

    // Configure driver class
    kv->put(driverClassKey, driverClass);

    // Acquire messaging access point instance through factory method
    boost::shared_ptr<MessagingAccessPoint> accessPoint = MessagingAccessPointFactory::getMessagingAccessPoint(accessPointUrl, kv);

    std::string queueName("TopicTest");

    boost::shared_ptr<KeyValue> subKV = OMS::newKeyValue();
    const std::string value = "OMS_CONSUMER";
    subKV->put(NonStandardKeys::CONSUMER_GROUP, value);

    boost::shared_ptr<consumer::PushConsumer> pushConsumer = accessPoint->createPushConsumer(subKV);

    // Create a listener instance
    boost::shared_ptr<MessageListener> messageListener = boost::make_shared<ExampleMessageListener>();

    // Attach listener to queue
    pushConsumer->attachQueue(queueName, messageListener);

    // Start push consumer
    pushConsumer->startup();

    LOG_INFO << "Push consumer starts OK";

    sleep(60 * 60);

}