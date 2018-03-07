#include <boost/smart_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <plog/Log.h>

#include <MessagingAccessPointFactory.h>
#include "core.h"
#include "NonStandardKeys.h"

using namespace std;
using namespace io::openmessaging;
using namespace io::openmessaging::producer;

int main(int argc, char *argv[]) {

    int count = 128;

    if (argc > 1) {
        count = boost::lexical_cast<int>(argv[1]);
    }

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


    boost::shared_ptr<consumer::PullConsumer> pullConsumer = accessPoint->createPullConsumer(queueName, subKV);

    pullConsumer->startup();

    while (true) {
        boost::shared_ptr<Message> msg = pullConsumer->poll();
        if (msg) {
            boost::shared_ptr<KeyValue> sysHeaders = msg->sysHeaders();
            std::string msgId = sysHeaders->getString(BuiltinKeys::MessageId);
            LOG_INFO << "Receive a new message. MsgId: " << msgId;
            pullConsumer->ack(msgId);
            LOG_INFO << "Acknowledging " << msgId << " OK";
        } else {
            LOG_INFO << "No new messages available. Sleep 5 seconds";
            sleep(5);
        }
    }

}