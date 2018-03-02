#include "consumer/StreamImpl.h"
#include "KeyValueImpl.h"

using namespace io::openmessaging;
using namespace io::openmessaging::consumer;


StreamImpl::StreamImpl(jobject proxy) : ServiceLifecycleImpl(proxy) {
    CurrentEnv ctx;
    const char *klassStream = "io/openmessaging/consumer/Stream";
    classStream = ctx.findClass(klassStream);
    midProperties = ctx.getMethodId(classStream, "properties", "()Lio/openmessaging/KeyValue;");
}

StreamImpl::~StreamImpl() {
    CurrentEnv ctx;
    ctx.deleteRef(classStream);
}

boost::shared_ptr<KeyValue> StreamImpl::properties() {
    CurrentEnv ctx;
    jobject kv = ctx.callObjectMethod(_proxy, midProperties);
    boost::shared_ptr<KeyValue> pKV = boost::make_shared<KeyValueImpl>(ctx.newGlobalRef(kv));
    ctx.deleteRef(kv);
    return pKV;
}

boost::shared_ptr<MessageIterator> StreamImpl::current() {
    throw OMSException("Not Implemented");
}

boost::shared_ptr<MessageIterator> StreamImpl::begin() {
    throw OMSException("Not Implemented");
}

boost::shared_ptr<MessageIterator> StreamImpl::end() {
    throw OMSException("Not Implemented");
}

boost::shared_ptr<MessageIterator> StreamImpl::seekByTime(long timestamp) {
    throw OMSException("Not Implemented");
}
