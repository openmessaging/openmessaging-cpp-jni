#include <map>
#include <boost/thread.hpp>

#include "consumer/PushConsumerImpl.h"
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

END_NAMESPACE_3(io, openmessaging, consumer)

PushConsumerImpl::PushConsumerImpl(jobject proxy) : ServiceLifecycleImpl(proxy) {
    CurrentEnv current;
    jclass classPushConsumerLocal = current.env->FindClass("io/openmessaging/consumer/PushConsumer");
    classPushConsumer = reinterpret_cast<jclass>(current.env->NewGlobalRef(classPushConsumerLocal));
    current.env->DeleteLocalRef(classPushConsumerLocal);

}

PushConsumerImpl::~PushConsumerImpl() {
    CurrentEnv current;
    current.env->DeleteGlobalRef(classPushConsumer);
}

boost::shared_ptr<KeyValue> PushConsumerImpl::properties() {

}

void PushConsumerImpl::resume() {

}

void PushConsumerImpl::suspend() {

}

bool PushConsumerImpl::isSuspended() {

}

PushConsumer &PushConsumerImpl::attachQueue(std::string &queueName,
                                            boost::shared_ptr<MessageListener> listener,
                                            boost::shared_ptr<KeyValue> properties) {


}

PushConsumer &PushConsumerImpl::detachQueue(std::string &queueName) {

}

void PushConsumerImpl::addInterceptor(boost::shared_ptr<PushConsumerInterceptor> interceptor) {

}

void PushConsumerImpl::removeInterceptor(boost::shared_ptr<PushConsumerInterceptor> interceptor) {

}
