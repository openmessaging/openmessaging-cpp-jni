#include "consumer/PushConsumerImpl.h"

BEGIN_NAMESPACE_3(io, openmessaging, consumer)

    using namespace io::openmessaging::consumer;

    PushConsumerImpl::PushConsumerImpl(jobject proxy) : objectPushConsumer(proxy), ServiceLifecycleImpl(proxy) {

    }

    boost::shared_ptr<KeyValue> PushConsumerImpl::properties() {

    }

    void PushConsumerImpl::resume() {

    }

    void PushConsumerImpl::suspend() {

    }

    bool PushConsumerImpl::isSuspended() {

    }

    PushConsumer& PushConsumerImpl::attachQueue(std::string &queueName,
                                  boost::shared_ptr<MessageListener> listener,
                                  boost::shared_ptr<KeyValue> properties) {

    }

    PushConsumer &PushConsumerImpl::detachQueue(std::string &queueName) {

    }

    void PushConsumerImpl::addInterceptor(boost::shared_ptr<PushConsumerInterceptor> interceptor) {

    }

    void PushConsumerImpl::removeInterceptor(boost::shared_ptr<PushConsumerInterceptor> interceptor) {

    }

END_NAMESPACE_3(io, openmessaging, consumer)