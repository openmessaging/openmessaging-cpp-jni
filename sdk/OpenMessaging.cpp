#include "OpenMessaging.h"
#include <string>
#include "plog/Log.h"

typedef io::openmessaging::producer::SendResult SR;
typedef io::openmessaging::producer::Producer P;
typedef io::openmessaging::ByteMessage BM;
typedef io::openmessaging::MessagingAccessPoint MAP;

struct AccessPoint* createAccessPoint(const char* accessUrl) {
    std::string url(accessUrl);
    io::openmessaging::MessagingAccessPointPtr ptr = getMessagingAccessPointImpl(url);
    MAP* p = ptr.raw();
    return reinterpret_cast<struct AccessPoint*>(p);
}


struct Producer* createProducer(struct AccessPoint* accessPoint) {
    MAP* ptr = reinterpret_cast<MAP *>(accessPoint);
    io::openmessaging::producer::ProducerPtr producerPtr = ptr->createProducer();
    struct Producer *p = reinterpret_cast<struct Producer*>(producerPtr.raw());
    return p;
}

int startProducer(struct Producer* producer) {
    P* ptr = reinterpret_cast<P*>(producer);
    try {
        ptr->startup();
        return 0;
    } catch (...) {
        LOG_ERROR << "Start producer failed";
    }
    return -1;
}

struct Message* createByteMessage(struct Producer* pro, const char* topic, signed char* body, unsigned int len) {
    P* ptr = reinterpret_cast<P*>(pro);
    io::openmessaging::MessageBody msgBody(body, len);
    io::openmessaging::ByteMessagePtr msgPtr = ptr->createBytesMessage(std::string(topic), msgBody);
    return reinterpret_cast<struct Message*>(msgPtr.raw());
}

struct SendResult* sendSync(struct Producer* producer, struct Message* message) {
    P* ptr = reinterpret_cast<P*>(producer);
    BM* msg = reinterpret_cast<BM*>(message);
    io::openmessaging::MessagePtr msgPtr(msg);
    try {
        io::openmessaging::producer::SendResultPtr sendResultPtr = ptr->send(msgPtr);
        return reinterpret_cast<struct SendResult*>(sendResultPtr.raw());
    } catch (...) {
        LOG_ERROR << "Sending message failed";
    }
    return NULL;
}

const char* msgId(struct SendResult* sendResult) {
    SR* sr = reinterpret_cast<SR*>(sendResult);
    return sr->messageId().c_str();
}

int shutdownProducer(struct Producer* producer) {
    P* ptr = reinterpret_cast<P*>(producer);
    try {
        ptr->shutdown();
    } catch (...) {
        return -1;
    }
    return 0;
}

void freeMessage(struct Message* message) {
    delete message;
}

void freeProducer(struct Producer* producer) {
    delete producer;
}