#include "consumer/PullConsumerImpl.h"
#include "KeyValueImpl.h"
#include "ByteMessageImpl.h"

using namespace io::openmessaging;
using namespace io::openmessaging::consumer;

PullConsumerImpl::PullConsumerImpl(jobject proxy) : ServiceLifecycleImpl(proxy) {
    CurrentEnv current;
    const char *klassPullConsumer = "io/openmessaging/consumer/PullConsumer";
    classPullConsumer = current.findClass(klassPullConsumer);

    midProperties = current.getMethodId(classPullConsumer, "properties", buildSignature(Types::KeyValue_, 0));
    midPoll = current.getMethodId(classPullConsumer, "poll", buildSignature(Types::Message_, 0));
    midPoll2 = current.getMethodId(classPullConsumer, "poll", buildSignature(Types::Message_, 1, Types::KeyValue_));
    midAck = current.getMethodId(classPullConsumer, "ack", buildSignature(Types::void_, 1, Types::String_));
    midAck2 = current.getMethodId(classPullConsumer, "ack", buildSignature(Types::void_, 2, Types::String_, Types::KeyValue_));
}

PullConsumerImpl::~PullConsumerImpl() {
    CurrentEnv current;
    current.deleteRef(classPullConsumer);
}

NS::shared_ptr<KeyValue> PullConsumerImpl::properties() {
    CurrentEnv current;
    jobject kv = current.callObjectMethod(_proxy, midProperties);
    NS::shared_ptr<KeyValueImpl> ptr = NS::make_shared<KeyValueImpl>(current.newGlobalRef(kv));
    current.deleteRef(kv);
    return ptr;
}

NS::shared_ptr<Message> PullConsumerImpl::poll(const NS::shared_ptr<KeyValue> &props) {
    CurrentEnv current;

    jobject jMessageLocal;
    if (props) {
        NS::shared_ptr<KeyValueImpl> ptr = NS::dynamic_pointer_cast<KeyValueImpl>(props);
        jMessageLocal = current.callObjectMethod(_proxy, midPoll2, ptr->getProxy());
    } else {
        jMessageLocal = current.callObjectMethod(_proxy, midPoll);
    }

    if (jMessageLocal) {
        jobject jMessage = current.newGlobalRef(jMessageLocal);
        NS::shared_ptr<Message> messagePtr = NS::make_shared<ByteMessageImpl>(jMessage);
        return messagePtr;
    }

    NS::shared_ptr<Message> msg_nullptr;
    return msg_nullptr;
}

void PullConsumerImpl::ack(const std::string &messageId,
                           const NS::shared_ptr<KeyValue> &props) {
    CurrentEnv current;

    jstring msgId = current.newStringUTF(messageId.c_str());
    if (props) {
        NS::shared_ptr<KeyValueImpl> ptr = NS::dynamic_pointer_cast<KeyValueImpl>(props);
        current.callObjectMethod(_proxy, midAck2, msgId, ptr->getProxy());
    } else {
        current.callObjectMethod(_proxy, midAck, msgId);
    }

    current.deleteRef(msgId);
}

jobject PullConsumerImpl::getProxy() {
    return _proxy;
}
