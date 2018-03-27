#include <plog/Log.h>

#include "MessagingAccessPointFactory.h"
#include "NonStandardKeys.h"

using namespace std;
using namespace io::openmessaging;
using namespace io::openmessaging::producer;

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
    const std::string consumer_group_value = "OMS_CONSUMER";
    subKV->put(CONSUMER_GROUP, consumer_group_value);


    NS::shared_ptr<consumer::PullConsumer> pullConsumer = accessPoint->createPullConsumer(queueName, subKV);

    pullConsumer->startup();

    while (true) {
        NS::shared_ptr<Message> msg = pullConsumer->poll();
        if (msg) {
            NS::shared_ptr<KeyValue> sysHeaders = msg->sysHeaders();
            std::string msgId = sysHeaders->getString(MessageId);
            LOG_INFO << "Receive a new message. MsgId: " << msgId;
            pullConsumer->ack(msgId);
            LOG_INFO << "Acknowledging " << msgId << " OK";
        } else {
            LOG_INFO << "No new messages available. Sleep 5 seconds";
            sleep(5);
        }
    }

}