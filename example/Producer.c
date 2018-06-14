#include "OpenMessaging.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {

    const char* accessUrl = "oms:rocketmq://ons3.dev:9876/default:default";
    struct AccessPoint* accessPoint = createAccessPoint(accessUrl);
    struct Producer* producer = createProducer(accessPoint);
    if (0 == startProducer(producer)) {
        const char* body = "Hello";
        struct Message* msg = createByteMessage(producer, "T_test", (signed char*)body, strlen(body));
        struct SendResult* sendResult = sendSync(producer, msg);
        if (NULL != sendResult) {
            printf("Message ID: %s\n", msgId(sendResult));
            shutdownProducer(producer);
            freeMessage(msg);
            return 0;
        } else {
            freeMessage(msg);
            printf("Sending message failed\n");
            return -1;
        }
    } else {
        printf("Starting producer failed\n");
    }

    if (NULL != producer) {
        freeProducer(producer);
        producer = NULL;
    }

    return -1;
}