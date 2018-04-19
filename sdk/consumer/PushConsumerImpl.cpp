#include <map>

#include "core.h"
#include "consumer/PushConsumerImpl.h"
#include "KeyValueImpl.h"
#include "ByteMessageImpl.h"
#include "consumer/ContextImpl.h"
#include "interceptor/ConsumerInterceptor.h"

using namespace io::openmessaging;
using namespace io::openmessaging::consumer;

BEGIN_NAMESPACE_3(io, openmessaging, consumer)
    boost::mutex listener_mutex;
    std::map<std::string, MessageListenerPtr> queue_listener_map;

    int interceptorIndex;
    boost::mutex interceptorMutex;
    std::map<int, interceptor::ConsumerInterceptorPtr> interceptorMap;

    void onMessage(JNIEnv *env, jobject object, jstring queue, jobject message, jobject context) {
        const char* queue_name_char_ptr = env->GetStringUTFChars(queue, NULL);
        LOG_DEBUG << "Incoming Message Queue Name: " << queue_name_char_ptr;
        std::string queue_name(queue_name_char_ptr);
        NS::shared_ptr<MessageListener> messageListenerPtr;
        {
            boost::lock_guard<boost::mutex> lk(listener_mutex);
            messageListenerPtr = queue_listener_map[queue_name];
        }

        if (messageListenerPtr) {
            LOG_DEBUG << "Message Listener Found";
            NS::shared_ptr<Message> messagePtr = NS::make_shared<ByteMessageImpl>(message);
            NS::shared_ptr<Context> contextPtr = NS::make_shared<ContextImpl>(context);
            messageListenerPtr->onMessage(messagePtr, contextPtr);
        }
    }

    void doPreReceive(JNIEnv *env, jobject object, jint index, jobject message, jobject attributes) {
        interceptor::ConsumerInterceptorPtr ptr;
        {
            boost::lock_guard<boost::mutex> lk(interceptorMutex);
            std::map<int, interceptor::ConsumerInterceptorPtr>::iterator search = interceptorMap.find(index);
            if (search != interceptorMap.end()) {
                ptr = search->second;
            } else {
                LOG_WARNING << "Interceptor is not found";
                return;
            }
        }

        ByteMessageImplPtr msg(new ByteMessageImpl(message));
        KeyValueImplPtr attr(new KeyValueImpl(attributes));
        ptr->postReceive(msg, attr);
    }

    void doPostReceive(JNIEnv *env, jobject object, jint index, jobject message, jobject attributes) {
        interceptor::ConsumerInterceptorPtr ptr;
        {
            boost::lock_guard<boost::mutex> lk(interceptorMutex);
            std::map<int, interceptor::ConsumerInterceptorPtr>::iterator search = interceptorMap.find(index);
            if (search != interceptorMap.end()) {
                ptr = search->second;
            } else {
                LOG_WARNING << "Interceptor is not found";
                return;
            }
        }

        ByteMessageImplPtr msg(new ByteMessageImpl(message));
        KeyValueImplPtr attr(new KeyValueImpl(attributes));
        ptr->preReceive(msg, attr);
    }

    static JNINativeMethod methods[] = {
        {
                const_cast<char*>("onMessage"),
                const_cast<char*>("(Ljava/lang/String;Lio/openmessaging/Message;Lio/openmessaging/consumer/MessageListener$Context;)V"),
                (void *)&onMessage
        }
    };

    static JNINativeMethod interceptorMethods[] = {
        {
                const_cast<char*>("doPreReceive"),
                const_cast<char*>("(ILio/openmessaging/Message;Lio/openmessaging/KeyValue;)V"),
                (void *)&doPreReceive
        },
        {
                const_cast<char *>("doPostReceive"),
                const_cast<char *>("(ILio/openmessaging/Message;Lio/openmessaging/KeyValue;)V"),
                (void *)&doPostReceive
        }
    };

END_NAMESPACE_3(io, openmessaging, consumer)

PushConsumerImpl::PushConsumerImpl(jobject proxy) : ServiceLifecycleImpl(proxy) {
    CurrentEnv current;

    const char *klassPushConsumer = "io/openmessaging/consumer/PushConsumer";
    const char *klassPushConsumerAdaptor = "io/openmessaging/consumer/PushConsumerAdaptor";
    const char *klassConsumerInterceptorAdaptor = "io/openmessaging/interceptor/ConsumerInterceptorAdaptor";
    const char *klassMessageListenerAdaptor = "io/openmessaging/consumer/MessageListenerAdaptor";

    classPushConsumer = current.findClass(klassPushConsumer);
    classPushConsumerAdaptor = current.findClass(klassPushConsumerAdaptor);
    classConsumerInterceptorAdaptor = current.findClass(klassConsumerInterceptorAdaptor);
    classMessageListenerAdaptor = current.findClass(klassMessageListenerAdaptor);

    if (current.env->RegisterNatives(classConsumerInterceptorAdaptor, interceptorMethods, 2) < 0) {
        LOG_ERROR << "Failed to register native methods for ConsumerInterceptorAdaptor";
        abort();
    }

    if (current.env->RegisterNatives(classMessageListenerAdaptor, methods, 1) < 0) {
        LOG_ERROR << "Failed to register native methods for MessageListenerAdaptor";
        abort();
    }

    midConsumerInterceptorAdaptor = current.getMethodId(classConsumerInterceptorAdaptor, "<init>",
                                                        buildSignature(Types::void_, 1, Types::int_));

    midPushConsumerAdaptor = current.getMethodId(classPushConsumerAdaptor, "<init>",
                                                 buildSignature(Types::void_, 1, Types::PushConsumer_));

    midAttributes = current.getMethodId(classPushConsumer, "attributes", buildSignature(Types::KeyValue_, 0));
    midResume = current.getMethodId(classPushConsumer, "resume", buildSignature(Types::void_, 0));
    midSuspend = current.getMethodId(classPushConsumer, "suspend", buildSignature(Types::void_, 0));
    midSuspend2 = current.getMethodId(classPushConsumer, "suspend", buildSignature(Types::void_, 1, Types::long_));
    midIsSuspended = current.getMethodId(classPushConsumer, "isSuspended", buildSignature(Types::boolean_, 0));
    midAttachQueue = current.getMethodId(classPushConsumer, "attachQueue",
                                         buildSignature(Types::PushConsumer_, 2, Types::String_, Types::MessageListener_));
    midAttachQueue2 = current.getMethodId(classPushConsumer, "attachQueue",
                                         buildSignature(Types::PushConsumer_, 3, Types::String_,
                                                        Types::MessageListener_, Types::KeyValue_));

    midDetachQueue = current.getMethodId(classPushConsumer, "detachQueue", buildSignature(Types::PushConsumer_, 1,
                                                                                          Types::String_));
    midAddInterceptor = current.getMethodId(classPushConsumerAdaptor, "addInterceptor",
                                            buildSignature(Types::void_, 2, Types::int_, Types::ConsumerInterceptor_));

    midRemoveInterceptor = current.getMethodId(classPushConsumerAdaptor, "removeInterceptor",
                                               buildSignature(Types::void_, 1, Types::int_));

    objectPushConsumerAdaptor = current.newObject(classPushConsumerAdaptor, midPushConsumerAdaptor, proxy);
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

    jmethodID ctor = ctx.getMethodId(classMessageListenerAdaptor, "<init>", buildSignature(Types::void_, 1, Types::String_));
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

void PushConsumerImpl::addInterceptor(const interceptor::ConsumerInterceptorPtr &interceptor) {
    CurrentEnv context;
    int index;
    {
        boost::lock_guard<boost::mutex> lk(interceptorMutex);
        index = ++interceptorIndex;
        interceptorMap[index] = interceptor;
    }

    jobject consumerInterceptor = context.newObject(classConsumerInterceptorAdaptor, midConsumerInterceptorAdaptor, index);
    context.callVoidMethod(objectPushConsumerAdaptor, midAddInterceptor, index, consumerInterceptor);
    context.deleteRef(consumerInterceptor);
    LOG_INFO << "Interceptor: " << interceptor->name() << " is added";
}

void PushConsumerImpl::removeInterceptor(const interceptor::ConsumerInterceptorPtr &interceptor) {
    CurrentEnv context;
    {
        boost::lock_guard<boost::mutex> lk(interceptorMutex);
        for(std::map<int, interceptor::ConsumerInterceptorPtr>::iterator it = interceptorMap.begin();
                it != interceptorMap.end(); it++) {
            if (it->second == interceptor) {
                context.callVoidMethod(objectPushConsumerAdaptor, midRemoveInterceptor, it->first);
                interceptorMap.erase(it->first);
                LOG_INFO << "Interceptor: " << interceptor->name() << " is removed";
            }
        }
    }
}
