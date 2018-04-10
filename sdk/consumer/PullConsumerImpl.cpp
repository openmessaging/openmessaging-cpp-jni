#include "consumer/PullConsumerImpl.h"
#include "KeyValueImpl.h"
#include "ByteMessageImpl.h"

using namespace io::openmessaging;
using namespace io::openmessaging::consumer;

PullConsumerImpl::PullConsumerImpl(jobject proxy) : ServiceLifecycleImpl(proxy) {
    CurrentEnv current;
    const char *klassPullConsumer = "io/openmessaging/consumer/PullConsumer";
    classPullConsumer = current.findClass(klassPullConsumer);

    midAttributes = current.getMethodId(classPullConsumer, "attributes", buildSignature(Types::void_, 0));
    midAttachQueue = current.getMethodId(classPullConsumer, "attachQueue", buildSignature(Types::PullConsumer_, 1, Types::String_));
    midAttachQueue2 = current.getMethodId(classPullConsumer, "attachQueue", buildSignature(Types::PullConsumer_, 2, Types::String_, Types::KeyValue_));
    midDetachQueue = current.getMethodId(classPullConsumer, "detachQueue", buildSignature(Types::PullConsumer_, 1, Types::String_));
    midReceive = current.getMethodId(classPullConsumer, "receive", buildSignature(Types::KeyValue_, 0));
    midReceive2 = current.getMethodId(classPullConsumer, "receive", buildSignature(Types::KeyValue_, 1, Types::KeyValue_));
    midAck = current.getMethodId(classPullConsumer, "ack", buildSignature(Types::void_, 1, Types::String_));
    midAck2 = current.getMethodId(classPullConsumer, "ack", buildSignature(Types::void_, 2, Types::String_, Types::KeyValue_));
}

PullConsumerImpl::~PullConsumerImpl() {
    CurrentEnv current;
    current.deleteRef(classPullConsumer);
}

KeyValuePtr PullConsumerImpl::attributes() {
    CurrentEnv current;
    jobject kv = current.callObjectMethod(_proxy, midAttributes);
    KeyValueImplPtr ptr = NS::make_shared<KeyValueImpl>(current.newGlobalRef(kv));
    current.deleteRef(kv);
    return ptr;
}

PullConsumer& PullConsumerImpl::attachQueue(const std::string &queueName, const KeyValuePtr &properties) {
    CurrentEnv current;
    jstring jQueueName = current.newStringUTF(queueName.c_str());
    jobject jPullConsumer;
    if (properties) {
        KeyValueImplPtr ptr = NS::dynamic_pointer_cast<KeyValueImpl>(properties);
        jPullConsumer = current.callObjectMethod(_proxy, midAttachQueue2, jQueueName, ptr->getProxy());
    } else {
        jPullConsumer = current.callObjectMethod(_proxy, midAttachQueue, jQueueName);
    }
    current.deleteRef(jQueueName);
    if (jPullConsumer) {
        current.deleteRef(jPullConsumer);
    }
    return *this;
}

PullConsumer& PullConsumerImpl::detachQueue(const std::string &queueName) {
    CurrentEnv context;
    jstring jQueueName = context.newStringUTF(queueName.c_str());
    jobject jPullConsumer = context.callObjectMethod(_proxy, midDetachQueue, jQueueName);
    context.deleteRef(jQueueName);
    if (jPullConsumer) {
        context.deleteRef(jPullConsumer);
    }
    return *this;
}

MessagePtr PullConsumerImpl::receive(const KeyValuePtr &props) {
    CurrentEnv current;

    jobject jMessageLocal;
    if (props) {
        KeyValueImplPtr ptr = NS::dynamic_pointer_cast<KeyValueImpl>(props);
        jMessageLocal = current.callObjectMethod(_proxy, midReceive2, ptr->getProxy());
    } else {
        jMessageLocal = current.callObjectMethod(_proxy, midReceive);
    }

    if (jMessageLocal) {
        jobject jMessage = current.newGlobalRef(jMessageLocal);
       MessagePtr messagePtr = NS::make_shared<ByteMessageImpl>(jMessage);
        return messagePtr;
    }

    MessagePtr msg_nullptr;
    return msg_nullptr;
}

void PullConsumerImpl::ack(const std::string &messageId, const KeyValuePtr &props) {
    CurrentEnv current;

    jstring msgId = current.newStringUTF(messageId.c_str());
    if (props) {
        KeyValueImplPtr ptr = NS::dynamic_pointer_cast<KeyValueImpl>(props);
        current.callObjectMethod(_proxy, midAck2, msgId, ptr->getProxy());
    } else {
        current.callObjectMethod(_proxy, midAck, msgId);
    }

    current.deleteRef(msgId);
}

jobject PullConsumerImpl::getProxy() {
    return _proxy;
}
