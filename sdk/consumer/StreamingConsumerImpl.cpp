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
