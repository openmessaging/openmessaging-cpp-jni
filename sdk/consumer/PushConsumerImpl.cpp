#include <map>
#include <boost/thread.hpp>

#include "consumer/PushConsumerImpl.h"
#include "KeyValueImpl.h"
#include "ByteMessage.h"

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
//            boost::shared_ptr<Message> messagePtr = boost::shared_ptr<ByteMessage>(message);

        }
    }

    static JNINativeMethod methods[] = {
            {
                    const_cast<char*>("onMessage"),
                    const_cast<char*>("(Ljava/lang/String;Lio/openmessaging/Message;Lio/openmessage/consumer/Context)V"),
                    (void *)&onMessage
            }
    };



END_NAMESPACE_3(io, openmessaging, consumer)

PushConsumerImpl::PushConsumerImpl(jobject proxy) : ServiceLifecycleImpl(proxy) {
    CurrentEnv current;
    jclass classPushConsumerLocal = current.env->FindClass("io/openmessaging/consumer/PushConsumer");
    classPushConsumer = current.makeGlobal(classPushConsumerLocal);

    jclass classMessageListenerAdaptorLocal = current.env->FindClass("io/openmessaging/consumer/MessageListenerAdaptor");
    classMessageListenerAdaptor = current.makeGlobal(classMessageListenerAdaptorLocal);
    if (current.env->RegisterNatives(classMessageListenerAdaptor, methods, 1) < 0) {
        BOOST_LOG_TRIVIAL(warning) << "Failed to register native methods";
        abort();
    }

    midProperties = current.env->GetMethodID(classPushConsumer, "properties", "()Lio/openmessaging/KeyValue;");
    midResume = current.env->GetMethodID(classPushConsumer, "resume", "()V");
    midSuspend = current.env->GetMethodID(classPushConsumer, "suspend", "()V");
    midIsSuspended = current.env->GetMethodID(classPushConsumer, "isSuspended", "()Z");
    const char *attachQueueSignature = "(Ljava/lang/String;Lio/openmessaging/consumer/MessageListener;)Lio/openmessaging/consumer/PushConsumer;";
    midAttachQueue = current.env->GetMethodID(classPushConsumer, "attachQueue", attachQueueSignature);

    midDetachQueue = current.env->GetMethodID(classPushConsumer, "detachQueue", "(Ljava/lang/String;)Lio/openmessaging/consumer/PushConsumer;");

}

PushConsumerImpl::~PushConsumerImpl() {
    CurrentEnv current;
    current.env->DeleteGlobalRef(classPushConsumer);
    current.env->DeleteGlobalRef(classMessageListenerAdaptor);
}

boost::shared_ptr<KeyValue> PushConsumerImpl::properties() {
    CurrentEnv ctx;
    jobject propertiesLocal = ctx.env->CallObjectMethod(_proxy, midProperties);
    jobject globalRef = ctx.makeGlobal(propertiesLocal);
    boost::shared_ptr<KeyValue> ret = boost::make_shared<KeyValueImpl>(globalRef);
    return ret;
}

void PushConsumerImpl::resume() {
    CurrentEnv ctx;
    ctx.env->CallVoidMethod(_proxy, midResume);
    ctx.checkAndClearException();
}

void PushConsumerImpl::suspend() {
    CurrentEnv ctx;
    ctx.env->CallVoidMethod(_proxy, midSuspend);
    ctx.checkAndClearException();
}

bool PushConsumerImpl::isSuspended() {
    CurrentEnv ctx;
    bool suspended = ctx.env->CallBooleanMethod(_proxy, midIsSuspended);
    ctx.checkAndClearException();
    return suspended;
}

PushConsumer &PushConsumerImpl::attachQueue(std::string &queueName,
                                            boost::shared_ptr<MessageListener> listener,
                                            boost::shared_ptr<KeyValue> properties) {
    CurrentEnv ctx;

    jmethodID ctor = ctx.env->GetMethodID(classMessageListenerAdaptor, "<init>", "(Ljava/lang/String;)V");
    jobject messageListener = ctx.env->NewObject(classMessageListenerAdaptor, ctor);
    const char* strs = queueName.c_str();
    jstring queue = ctx.env->NewStringUTF(strs);
    {
        boost::lock_guard<boost::mutex> lk(listener_mutex);
        queue_listener_map[queueName] = listener;
    }

    jobject ret = ctx.env->CallObjectMethod(_proxy, midAttachQueue, queue, messageListener);
    ctx.checkAndClearException();
    ctx.env->DeleteLocalRef(ret);

    ctx.env->ReleaseStringUTFChars(queue, strs);
    return *this;
}

PushConsumer &PushConsumerImpl::detachQueue(std::string &queueName) {

    {
        boost::lock_guard<boost::mutex> lk(listener_mutex);
        queue_listener_map.erase(queueName);
    }

    CurrentEnv ctx;
    const char *str = queueName.c_str();
    jstring queue = ctx.env->NewStringUTF(str);
    jobject ret = ctx.env->CallObjectMethod(_proxy, midDetachQueue, queue);
    ctx.checkAndClearException();
    ctx.deleteLocalRef(ret);
    ctx.env->ReleaseStringUTFChars(queue, str);
    return *this;
}

void PushConsumerImpl::addInterceptor(boost::shared_ptr<PushConsumerInterceptor> interceptor) {

}

void PushConsumerImpl::removeInterceptor(boost::shared_ptr<PushConsumerInterceptor> interceptor) {

}
