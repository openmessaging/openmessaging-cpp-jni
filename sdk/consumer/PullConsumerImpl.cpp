#include "consumer/PullConsumerImpl.h"
#include "KeyValueImpl.h"
#include "ByteMessageImpl.h"

BEGIN_NAMESPACE_3(io, openmessaging, consumer)

    PullConsumerImpl::PullConsumerImpl(jobject proxy) : objectPullConsumer(proxy), ServiceLifecycleImpl(proxy) {
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

    boost::shared_ptr<KeyValue> PullConsumerImpl::properties() {
        CurrentEnv current;
        jobject kv = current.env->CallObjectMethod(objectPullConsumer, midProperties);
        boost::shared_ptr<KeyValueImpl> ptr = boost::make_shared<KeyValueImpl>(current.env->NewGlobalRef(kv));
        current.env->DeleteLocalRef(kv);
        return ptr;
    }

    boost::shared_ptr<Message> PullConsumerImpl::poll(boost::shared_ptr<KeyValue> properties) {
        CurrentEnv current;
        jobject jMessage;
        if (!properties) {
            boost::shared_ptr<KeyValueImpl> ptr = boost::dynamic_pointer_cast<KeyValueImpl>(properties);
            jMessage = current.env->CallObjectMethod(objectPullConsumer, midPoll2, ptr->getInternal());
        } else {
            jMessage = current.env->CallObjectMethod(objectPullConsumer, midPoll);
        }
        boost::shared_ptr<Message> pMessage = boost::make_shared<ByteMessageImpl>(jMessage);
        return pMessage;
    }

    void PullConsumerImpl::ack(const std::string &messageId, boost::shared_ptr<KeyValue> properties) {
        CurrentEnv current;
        jstring msgId = current.env->NewStringUTF(messageId.c_str());
        if (!properties) {
            boost::shared_ptr<KeyValueImpl> ptr = boost::dynamic_pointer_cast<KeyValueImpl>(properties);
            current.env->CallObjectMethod(objectPullConsumer, midAck2, msgId, ptr->getInternal());
        } else {
            current.env->CallObjectMethod(objectPullConsumer, midAck, msgId);
        }
        current.env->DeleteLocalRef(msgId);
    }

    jobject PullConsumerImpl::getProxy() {
        return objectPullConsumer;
    }

END_NAMESPACE_3(io, openmessaging, consumer)