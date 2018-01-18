#include "consumer/StreamingConsumerImpl.h"

BEGIN_NAMESPACE_3(io, openmessaging, consumer)
    StreamingConsumerImpl::StreamingConsumerImpl(jobject proxy) : objectStreamingConsumer(proxy),
                                                                  ServiceLifecycleImpl(proxy) {

    }

    boost::shared_ptr<KeyValue> StreamingConsumerImpl::properties() {

    }

    std::vector<std::string> StreamingConsumerImpl::streams() {

    }

    std::vector<std::string> StreamingConsumerImpl::consumers() {

    }

    consumer::MessageIterator StreamingConsumerImpl::current() {

    }

    consumer::MessageIterator StreamingConsumerImpl::begin() {

    }

    consumer::MessageIterator StreamingConsumerImpl::end() {

    }

    consumer::MessageIterator StreamingConsumerImpl::seekByTime(long timestamp) {

    }

END_NAMESPACE_3(io, openmessaging, consumer)