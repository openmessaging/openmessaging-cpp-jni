#include "consumer/StreamingConsumerImpl.h"

using namespace io::openmessaging;
using namespace io::openmessaging::consumer;

StreamingConsumerImpl::StreamingConsumerImpl(jobject proxy) : ServiceLifecycleImpl(proxy) {
    CurrentEnv current;
    const char *klassStreamingConsumer = "io/openmessaging/consumer/StreamingConsumer";
    classStreamingConsumer = current.findClass(klassStreamingConsumer);
}

StreamingConsumerImpl::~StreamingConsumerImpl() {
    CurrentEnv current;
    current.deleteRef(classStreamingConsumer);
}

boost::shared_ptr<KeyValue> StreamingConsumerImpl::properties() {
    throw OMSException("Not Implemented");
}

std::vector<std::string> StreamingConsumerImpl::streams() {
    throw OMSException("Not Implemented");
}

std::vector<std::string> StreamingConsumerImpl::consumers() {
    throw OMSException("Not Implemented");
}

boost::shared_ptr<MessageIterator> StreamingConsumerImpl::current() {
    throw OMSException("Not Implemented");
}

boost::shared_ptr<MessageIterator> StreamingConsumerImpl::begin() {
    throw OMSException("Not Implemented");
}

boost::shared_ptr<MessageIterator> StreamingConsumerImpl::end() {
    throw OMSException("Not Implemented");
}

boost::shared_ptr<MessageIterator> StreamingConsumerImpl::seekByTime(long timestamp) {
    throw OMSException("Not Implemented");
}
