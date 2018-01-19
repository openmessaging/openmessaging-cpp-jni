#include "consumer/StreamingConsumerImpl.h"

BEGIN_NAMESPACE_3(io, openmessaging, consumer)
    StreamingConsumerImpl::StreamingConsumerImpl(jobject proxy) : ServiceLifecycleImpl(proxy) {
        CurrentEnv current;
        jclass classStreamingConsumerLocal = current.env->FindClass("io/openmessaging/consumer/StreamingConsumer");
        classStreamingConsumer = reinterpret_cast<jclass>(current.env->NewGlobalRef(classStreamingConsumerLocal));
        current.env->DeleteLocalRef(classStreamingConsumerLocal);

    }

    StreamingConsumerImpl::~StreamingConsumerImpl() {
        CurrentEnv current;
        current.env->DeleteGlobalRef(classStreamingConsumer);
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