#ifndef OMS_OPEN_MESSAGING_H
#define OMS_OPEN_MESSAGING_H

#ifdef __cplusplus

#include "OMS.h"
#include "MessagingAccessPoint.h"
#include "BuiltinKeys.h"

extern "C" {
#endif

    struct AccessPoint;

    struct Message;

    struct Producer;

    struct SendResult;

    struct AccessPoint* createAccessPoint(const char* accessUrl);

    struct Producer* createProducer(struct AccessPoint* accessPoint);

    struct Message* createByteMessage(struct Producer*, const char* topic, signed char* body, unsigned int len);

    struct SendResult* sendSync(struct Producer* producer, struct Message* message);

    const char* msgId(struct SendResult* sendResult);

#ifdef __cplusplus
}
#endif


#endif //OMS_OPEN_MESSAGING_H
