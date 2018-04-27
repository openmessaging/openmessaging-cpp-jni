#include "consumer/PullConsumerImpl.h"
#include "KeyValueImpl.h"
#include "ByteMessageImpl.h"

using namespace io::openmessaging;
using namespace io::openmessaging::consumer;

PullConsumerImpl::PullConsumerImpl(jobject proxy) : ServiceLifecycleImpl(proxy) {
    CurrentEnv current;
    const char *klassPullConsumer = "io/openmessaging/consumer/PullConsumer";
    classPullConsumer = current.findClass(klassPullConsumer);

    midAttributes = current.getMethodId(classPullConsumer, "attributes", buildSignature(Types::KeyValue_, 0));
    midAttachQueue = current.getMethodId(classPullConsumer, "attachQueue", buildSignature(Types::PullConsumer_, 1, Types::String_));
    midAttachQueue2 = current.getMethodId(classPullConsumer, "attachQueue", buildSignature(Types::PullConsumer_, 2, Types::String_, Types::KeyValue_));
    midDetachQueue = current.getMethodId(classPullConsumer, "detachQueue", buildSignature(Types::PullConsumer_, 1, Types::String_));
    midReceive = current.getMethodId(classPullConsumer, "receive", buildSignature(Types::Message_, 0));
    midReceive2 = current.getMethodId(classPullConsumer, "receive", buildSignature(Types::Message_, 1, Types::KeyValue_));
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
        jobject kv = (dynamic_cast<KeyValueImpl*>(properties.get()))->getProxy();
        jPullConsumer = current.callObjectMethod(_proxy, midAttachQueue2, jQueueName, kv);
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
        jobject kv = (dynamic_cast<KeyValueImpl*>(props.get()))->getProxy();
        jMessageLocal = current.callObjectMethod(_proxy, midReceive2, kv);
    } else {
        jMessageLocal = current.callObjectMethod(_proxy, midReceive);
    }

    if (jMessageLocal) {
        jobject jMessage = current.newGlobalRef(jMessageLocal);
       MessagePtr messagePtr = NS::make_shared<ByteMessageImpl>(jMessage);
        return messagePtr;
    } else {
        LOG_DEBUG << "Receive returns without any message";
    }

    MessagePtr msg_nullptr;
    return msg_nullptr;
}

void PullConsumerImpl::ack(const std::string &messageId, const KeyValuePtr &props) {
    CurrentEnv current;

    jstring msgId = current.newStringUTF(messageId.c_str());
    if (props) {
        jobject kv = (dynamic_cast<KeyValueImpl*>(props.get()))->getProxy();
        current.callObjectMethod(_proxy, midAck2, msgId, kv);
    } else {
        current.callObjectMethod(_proxy, midAck, msgId);
    }

    current.deleteRef(msgId);
}

jobject PullConsumerImpl::getProxy() {
    return _proxy;
}
