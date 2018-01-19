#include "consumer/PushConsumerImpl.h"

using namespace io::openmessaging;
using namespace io::openmessaging::consumer;

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
