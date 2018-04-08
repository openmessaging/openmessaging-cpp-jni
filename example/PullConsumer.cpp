#include <plog/Log.h>

#include "NonStandardKeys.h"
#include "OMS.h"
#include "consumer/PullConsumer.h"
#include "MessagingAccessPoint.h"


using namespace std;
using namespace io::openmessaging;

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
    const std::string consumer_group_value = "OMS_CONSUMER";
    subKV->put(CONSUMER_GROUP, consumer_group_value);


    consumer::PullConsumerPtr pullConsumer = accessPoint->createPullConsumer(queueName, subKV);

    pullConsumer->startup();

    while (true) {
        MessagePtr msg = pullConsumer->receive();
        if (msg) {
            KeyValuePtr sysHeaders = msg->sysHeaders();
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