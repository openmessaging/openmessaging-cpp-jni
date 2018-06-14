#include "OpenMessaging.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {

    const char* accessUrl = "oms:rocketmq://ons3.dev:9876/default:default";
    struct AccessPoint* accessPoint = createAccessPoint(accessUrl);
    struct Producer* producer = createProducer(accessPoint);
    const char* body = "Hello";
    struct Message* msg = createByteMessage(producer, "T_test", (signed char*)body, strlen(body));
    struct SendResult* sendResult = sendSync(producer, msg);
    printf("Message ID: %s\n", msgId(sendResult));

}