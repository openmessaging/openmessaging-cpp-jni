#include <iostream>

#include <boost/smart_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <MessagingAccessPointFactory.h>
#include "core.h"

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

    // Create a producer instance via MessageAccessPoint instance
    boost::shared_ptr<Producer> producer = accessPoint->createProducer();

    // Start producer and hereafter be prepared to send message to brokers
    producer->startup();

    // Now that producers are ready to deliver, we are about to create messages
    const string topic = "TopicTest";
    const char *slogan = "OpenMessagingService is the NEXT generation of messaging system";
    char* buffer = new char[strlen(slogan) + 1];
    strcpy(buffer, slogan);

    const scoped_array<char> message_body(buffer, strlen(buffer));

    // Create message by producer, which also plays the role of message factory
    boost::shared_ptr<ByteMessage> message = producer->createByteMessageToTopic(topic, message_body);

    for (int i = 0; i < count; ++i) {
        // Send message
        boost::shared_ptr<SendResult> sendResult = producer->send(message);

        // SendResult instance holds meta data including message id which may be employed to query various info on messaging
        // console
        cout << "Sending Message OK. Message ID: " << sendResult->messageId() << endl;
    }


    // Shutdown producer if it's no longer used. Typically is called when your application is about to quit.
    producer->shutdown();
}