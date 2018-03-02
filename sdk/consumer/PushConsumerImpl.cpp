#include <map>
#include <boost/thread.hpp>

#include "consumer/PushConsumerImpl.h"
#include "KeyValueImpl.h"
#include "ByteMessageImpl.h"
#include "consumer/ContextImpl.h"

using namespace io::openmessaging;
using namespace io::openmessaging::consumer;

BEGIN_NAMESPACE_3(io, openmessaging, consumer)
    boost::mutex listener_mutex;
    std::map<std::string, boost::shared_ptr<MessageListener> > queue_listener_map;

    void onMessage(JNIEnv *env, jobject object, jstring queue, jobject message, jobject context) {
        const char* queue_name_char_ptr = env->GetStringUTFChars(queue, NULL);
        std::string queue_name(queue_name_char_ptr);
        boost::shared_ptr<MessageListener> messageListenerPtr;
        {
            boost::lock_guard<boost::mutex> lk(listener_mutex);
            messageListenerPtr = queue_listener_map[queue_name];
        }

        if (messageListenerPtr) {
            boost::shared_ptr<Message> messagePtr = boost::make_shared<ByteMessageImpl>(message);
            boost::shared_ptr<Context> contextPtr = boost::make_shared<ContextImpl>(context);
            messageListenerPtr->onMessage(messagePtr, contextPtr);
        }
    }

    static JNINativeMethod methods[] = {
            {
                    const_cast<char*>("onMessage"),
                    const_cast<char*>("(Ljava/lang/String;Lio/openmessaging/Message;Lio/openmessaging/consumer/Context;)V"),
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
        BOOST_LOG_TRIVIAL(warning) << "Failed to register native methods";
        abort();
    }

    midProperties = current.getMethodId(classPushConsumer, "properties", "()Lio/openmessaging/KeyValue;");
    midResume = current.getMethodId(classPushConsumer, "resume", "()V");
    midSuspend = current.getMethodId(classPushConsumer, "suspend", "()V");
    midIsSuspended = current.getMethodId(classPushConsumer, "isSuspended", "()Z");

    const char *attachQueueSignature = "(Ljava/lang/String;Lio/openmessaging/consumer/MessageListener;)Lio/openmessaging/consumer/PushConsumer;";
    midAttachQueue = current.getMethodId(classPushConsumer, "attachQueue", attachQueueSignature);

    midDetachQueue = current.getMethodId(classPushConsumer, "detachQueue", "(Ljava/lang/String;)Lio/openmessaging/consumer/PushConsumer;");

}

PushConsumerImpl::~PushConsumerImpl() {
    CurrentEnv current;
    current.deleteRef(classPushConsumer);
    current.deleteRef(classMessageListenerAdaptor);
}

boost::shared_ptr<KeyValue> PushConsumerImpl::properties() {
    CurrentEnv ctx;
    jobject propertiesLocal = ctx.callObjectMethod(_proxy, midProperties);
    jobject globalRef = ctx.newGlobalRef(propertiesLocal);
    boost::shared_ptr<KeyValue> ret = boost::make_shared<KeyValueImpl>(globalRef);
    return ret;
}

void PushConsumerImpl::resume() {
    CurrentEnv ctx;
    ctx.callVoidMethod(_proxy, midResume);
}

void PushConsumerImpl::suspend() {
    CurrentEnv ctx;
    ctx.callVoidMethod(_proxy, midSuspend);
}

bool PushConsumerImpl::isSuspended() {
    CurrentEnv ctx;
    bool suspended = ctx.callBooleanMethod(_proxy, midIsSuspended);
    return suspended;
}

PushConsumer &PushConsumerImpl::attachQueue(const std::string &queueName,
                                            const boost::shared_ptr<MessageListener> &listener,
                                            const boost::shared_ptr<KeyValue> &props) {
    CurrentEnv ctx;

    jmethodID ctor = ctx.env->GetMethodID(classMessageListenerAdaptor, "<init>", "(Ljava/lang/String;)V");
    const char* queueNameChars = queueName.c_str();
    jstring jQueueName = ctx.env->NewStringUTF(queueNameChars);
    jobject messageListener = ctx.newObject(classMessageListenerAdaptor, ctor, jQueueName);
    BOOST_LOG_TRIVIAL(debug) << "MessageListenerAdaptor instance created";
    {
        boost::lock_guard<boost::mutex> lk(listener_mutex);
        queue_listener_map[queueName] = listener;
    }

    jobject ret = ctx.callObjectMethod(_proxy, midAttachQueue, jQueueName, messageListener);
    BOOST_LOG_TRIVIAL(debug) << "MessageListenerAdaptor per queue attached";
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
    const char *str = queueName.c_str();
    jstring queue = ctx.env->NewStringUTF(str);
    jobject ret = ctx.callObjectMethod(_proxy, midDetachQueue, queue);
    ctx.deleteRef(ret);
    ctx.env->ReleaseStringUTFChars(queue, str);
    return *this;
}

void PushConsumerImpl::addInterceptor(const boost::shared_ptr<PushConsumerInterceptor> &interceptor) {
    throw OMSException("Not Implemented");
}

void PushConsumerImpl::removeInterceptor(const boost::shared_ptr<PushConsumerInterceptor> &interceptor) {
    throw OMSException("Not Implemented");
}
