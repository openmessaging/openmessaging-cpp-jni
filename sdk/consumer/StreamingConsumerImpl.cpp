#include "consumer/StreamingConsumerImpl.h"

using namespace io::openmessaging;
using namespace io::openmessaging::consumer;

StreamingConsumerImpl::StreamingConsumerImpl(jobject proxy) : ServiceLifecycleImpl(proxy) {
    CurrentEnv ctx;
    const char *klassStreamingConsumer = "io/openmessaging/consumer/StreamingConsumer";
    classStreamingConsumer = ctx.findClass(klassStreamingConsumer);
}

StreamingConsumerImpl::~StreamingConsumerImpl() {
    CurrentEnv ctx;
    ctx.deleteRef(classStreamingConsumer);
}

KeyValuePtr StreamingConsumerImpl::attributes() {
    throw OMSException("Not Implemented");
}

StreamingIteratorPtr StreamingConsumerImpl::seek(const std::string &name, long offset, int whence) {

}