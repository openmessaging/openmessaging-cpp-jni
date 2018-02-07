#include "consumer/PullConsumerImpl.h"
#include "KeyValueImpl.h"
#include "ByteMessageImpl.h"

using namespace io::openmessaging;
using namespace io::openmessaging::consumer;

PullConsumerImpl::PullConsumerImpl(jobject proxy) : ServiceLifecycleImpl(proxy) {
    CurrentEnv current;
    jclass classPullConsumerLocal = current.env->FindClass("io/openmessaging/consumer/PullConsumer");
    classPullConsumer = reinterpret_cast<jclass>(current.env->NewGlobalRef(classPullConsumerLocal));
    current.env->DeleteLocalRef(classPullConsumerLocal);

    midProperties = getMethod(current, classPullConsumer, "properties", "()Lio/openmessaging/KeyValue;");
    midPoll = getMethod(current, classPullConsumer, "poll", "()Lio/openmessaging/Message;");
    midPoll2 = getMethod(current, classPullConsumer, "poll", "(Lio/openmessaging/KeyValue;)Lio/openmessaging/Message;");
    midAck = getMethod(current, classPullConsumer, "ack", "(Ljava/lang/String;)V");
    midAck2 = getMethod(current, classPullConsumer, "ack", "(Ljava/lang/String;Lio/openmessaging/KeyValue;)V");
}

PullConsumerImpl::~PullConsumerImpl() {
    CurrentEnv current;
    current.env->DeleteGlobalRef(classPullConsumer);
}

boost::shared_ptr<KeyValue> PullConsumerImpl::properties() {
    CurrentEnv current;
    jobject kv = current.env->CallObjectMethod(_proxy, midProperties);
    boost::shared_ptr<KeyValueImpl> ptr = boost::make_shared<KeyValueImpl>(current.env->NewGlobalRef(kv));
    current.env->DeleteLocalRef(kv);
    return ptr;
}

boost::shared_ptr<Message> PullConsumerImpl::poll(boost::shared_ptr<KeyValue> properties) {
    CurrentEnv current;
    jobject jMessageLocal;
    if (properties) {
        boost::shared_ptr<KeyValueImpl> ptr = boost::dynamic_pointer_cast<KeyValueImpl>(properties);
        jMessageLocal = current.env->CallObjectMethod(_proxy, midPoll2, ptr->getProxy());
    } else {
        jMessageLocal = current.env->CallObjectMethod(_proxy, midPoll);
    }

    if (current.checkAndClearException()) {
        abort();
    }

    if (jMessageLocal) {
        jobject jMessage = current.env->NewGlobalRef(jMessageLocal);
        current.env->DeleteLocalRef(jMessageLocal);
        boost::shared_ptr<Message> messagePtr = boost::make_shared<ByteMessageImpl>(jMessage);
        return messagePtr;
    }

    boost::shared_ptr<Message> msg_nullptr;
    return msg_nullptr;
}

void PullConsumerImpl::ack(const std::string &messageId, boost::shared_ptr<KeyValue> properties) {
    CurrentEnv current;
    jstring msgId = current.env->NewStringUTF(messageId.c_str());
    if (properties) {
        boost::shared_ptr<KeyValueImpl> ptr = boost::dynamic_pointer_cast<KeyValueImpl>(properties);
        current.env->CallObjectMethod(_proxy, midAck2, msgId, ptr->getProxy());
    } else {
        current.env->CallObjectMethod(_proxy, midAck, msgId);
    }

    if (current.checkAndClearException()) {
        abort();
    }

    current.env->DeleteLocalRef(msgId);
}

jobject PullConsumerImpl::getProxy() {
    return _proxy;
}
