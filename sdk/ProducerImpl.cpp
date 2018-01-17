#include "ProducerImpl.h"

BEGIN_NAMESPACE_3(io, openmessaging, core)

    ProducerImpl::ProducerImpl(jobject proxy, boost::shared_ptr<KeyValue> properties)
            : objectProducer(proxy), _properties(properties) {

    }

    ProducerImpl::~ProducerImpl() {
        CurrentEnv current;
        current.env->DeleteGlobalRef(objectProducer);
    }

    boost::shared_ptr<KeyValue> ProducerImpl::properties() {
        return _properties;
    }

    boost::shared_ptr<producer::SendResult> send(boost::shared_ptr<Message> message,
                                       boost::shared_ptr<KeyValue> properties) {

    }

END_NAMESPACE_3(io, openmessaging, core)
