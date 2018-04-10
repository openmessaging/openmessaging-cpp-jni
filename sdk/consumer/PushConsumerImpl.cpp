#include <map>

#include "core.h"
#include "consumer/PushConsumerImpl.h"
#include "KeyValueImpl.h"
#include "ByteMessageImpl.h"
#include "consumer/ContextImpl.h"

using namespace io::openmessaging;
using namespace io::openmessaging::consumer;

BEGIN_NAMESPACE_3(io, openmessaging, consumer)
    boost::mutex listener_mutex;
    std::map<std::string, NS::shared_ptr<MessageListener> > queue_listener_map;

    void onMessage(JNIEnv *env, jobject object, jstring queue, jobject message, jobject context) {
        const char* queue_name_char_ptr = env->GetStringUTFChars(queue, NULL);
        std::string queue_name(queue_name_char_ptr);
        NS::shared_ptr<MessageListener> messageListenerPtr;
        {
            boost::lock_guard<boost::mutex> lk(listener_mutex);
            messageListenerPtr = queue_listener_map[queue_name];
        }

        if (messageListenerPtr) {
            NS::shared_ptr<Message> messagePtr = NS::make_shared<ByteMessageImpl>(message);
            NS::shared_ptr<Context> contextPtr = NS::make_shared<ContextImpl>(context);
            messageListenerPtr->onMessage(messagePtr, contextPtr);
        }
    }

    const std::string signature = buildSignature(Types::void_, 3, Types::String_, Types::Message_, Types::Context_);
    static JNINativeMethod methods[] = {
            {
                    const_cast<char*>("onMessage"),
                    const_cast<char*>(signature.c_str()),
                    (void *)&onMessage
            }
    };



END_NAMESPACE_3(io, openmessaging, consumer)

PushConsumerImpl::PushConsumerImpl(jobject proxy) : ServiceLifecycleImpl(proxy) {
    CurrentEnv current;

    const char *klassPushConsumer = "io/openmessaging/consumer/PushConsumer";
    classPushConsumer = current.findClass(klassPushConsumer);

    const char *klassMessageListenerAdaptor = "io/openmessaging/consumer/MessageListenerAdaptor";
    classMessageListenerAdaptor = current.findClass(klassMessageListenerAdaptor);

    if (current.env->RegisterNatives(classMessageListenerAdaptor, methods, 1) < 0) {
        LOG_WARNING << "Failed to register native methods";
        abort();
    }

    midAttributes = current.getMethodId(classPushConsumer, "attributes", buildSignature(Types::KeyValue_, 0));
    midResume = current.getMethodId(classPushConsumer, "resume", buildSignature(Types::void_, 0));
    midSuspend = current.getMethodId(classPushConsumer, "suspend", buildSignature(Types::long_, 0));
    midSuspend2 = current.getMethodId(classPushConsumer, "suspend", buildSignature(Types::long_, 1, Types::long_));
    midIsSuspended = current.getMethodId(classPushConsumer, "isSuspended", buildSignature(Types::boolean_, 0));
    midAttachQueue = current.getMethodId(classPushConsumer, "attachQueue",
                                         buildSignature(Types::PushConsumer_, 2, Types::String_, Types::MessageListener_));
    midAttachQueue2 = current.getMethodId(classPushConsumer, "attachQueue",
                                         buildSignature(Types::PushConsumer_, 3, Types::String_,
                                                        Types::MessageListener_, Types::KeyValue_));

    midDetachQueue = current.getMethodId(classPushConsumer, "detachQueue", buildSignature(Types::PushConsumer_, 1,
                                                                                          Types::String_));
    midAddInterceptor = current.getMethodId(classPushConsumer, "addInterceptor",
                                            buildSignature(Types::void_, 1, Types::ConsumerInterceptor_));
    midRemoveInterceptor = current.getMethodId(classPushConsumer, "removeInterceptor",
                                               buildSignature(Types::void_, 1, Types::ConsumerInterceptor_));
}

PushConsumerImpl::~PushConsumerImpl() {
    CurrentEnv current;
    current.deleteRef(classPushConsumer);
    current.deleteRef(classMessageListenerAdaptor);
}

KeyValuePtr PushConsumerImpl::attributes() {
    CurrentEnv ctx;
    jobject propertiesLocal = ctx.callObjectMethod(_proxy, midAttributes);
    jobject globalRef = ctx.newGlobalRef(propertiesLocal);
    KeyValuePtr ret = NS::make_shared<KeyValueImpl>(globalRef);
    return ret;
}

void PushConsumerImpl::resume() {
    CurrentEnv ctx;
    ctx.callVoidMethod(_proxy, midResume);
}

void PushConsumerImpl::suspend(long timeout) {
    CurrentEnv ctx;
    ctx.callVoidMethod(_proxy, midSuspend, timeout);
}

bool PushConsumerImpl::isSuspended() {
    CurrentEnv ctx;
    bool suspended = ctx.callBooleanMethod(_proxy, midIsSuspended);
    return suspended;
}

PushConsumer &PushConsumerImpl::attachQueue(const std::string &queueName,
                                            const MessageListenerPtr &listener,
                                            const KeyValuePtr &props) {
    CurrentEnv ctx;

    jmethodID ctor = ctx.getMethodId(classMessageListenerAdaptor, "<init>", buildSignature(Types::String_, 0));
    const char* queueNameChars = queueName.c_str();
    jstring jQueueName = ctx.newStringUTF(queueNameChars);
    jobject messageListener = ctx.newObject(classMessageListenerAdaptor, ctor, jQueueName);
    LOG_DEBUG << "MessageListenerAdaptor instance created";
    {
        boost::lock_guard<boost::mutex> lk(listener_mutex);
        queue_listener_map[queueName] = listener;
    }

    jobject ret;
    if (props) {
        KeyValueImplPtr ptr = NS::dynamic_pointer_cast<KeyValueImpl>(props);
        ret = ctx.callObjectMethod(_proxy, midAttachQueue2, jQueueName, messageListener, ptr->getProxy());
    } else {
        ret = ctx.callObjectMethod(_proxy, midAttachQueue, jQueueName, messageListener);
    }
    LOG_DEBUG << "MessageListenerAdaptor per queue attached";
    ctx.deleteRef(messageListener);
    ctx.deleteRef(jQueueName);
    ctx.deleteRef(ret);
    return *this;
}

PushConsumer &PushConsumerImpl::detachQueue(const std::string &queueName) {

    {
        boost::lock_guard<boost::mutex> lk(listener_mutex);
        queue_listener_map.erase(queueName);
    }

    CurrentEnv ctx;
    jstring queue = ctx.newStringUTF(queueName.c_str());
    jobject ret = ctx.callObjectMethod(_proxy, midDetachQueue, queue);
    ctx.deleteRef(ret);
    ctx.deleteRef(queue);
    return *this;
}

void PushConsumerImpl::addInterceptor(const PushConsumerInterceptorPtr &interceptor) {
    throw OMSException("Not Implemented");
}

void PushConsumerImpl::removeInterceptor(const PushConsumerInterceptorPtr &interceptor) {
    throw OMSException("Not Implemented");
}
