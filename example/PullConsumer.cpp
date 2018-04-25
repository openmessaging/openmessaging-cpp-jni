#include <plog/Log.h>

#include <OpenMessaging.h>

using namespace std;
using namespace io::openmessaging;

int main(int argc, char *argv[]) {

    // Define access point
    const string accessPointUrl = "oms:rocketmq://ons3.dev:9876/default:default";

    // Acquire messaging access point instance through factory method
    MessagingAccessPointPtr accessPoint(getMessagingAccessPoint(accessPointUrl));

    KeyValuePtr subKV(newKeyValue());
    subKV->put(CONSUMER_ID, "OMS_CONSUMER");
    consumer::PullConsumerPtr pullConsumer = accessPoint->createPullConsumer(subKV);

    pullConsumer->attachQueue("TopicTest");

    pullConsumer->startup();

    while (true) {
        MessagePtr msg = pullConsumer->receive();
        if (msg) {
            KeyValuePtr sysHeaders = msg->sysHeaders();
            std::string msgId = sysHeaders->getString(MESSAGE_ID);
            LOG_INFO << "Receive a new message. MsgId: " << msgId;
            pullConsumer->ack(msgId);
            LOG_INFO << "Acknowledging " << msgId << " OK";
        } else {
            LOG_INFO << "No new messages available. Sleep 5 seconds";
            sleep(5);
        }
    }

}