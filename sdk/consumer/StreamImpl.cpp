#include "consumer/StreamImpl.h"
#include "KeyValueImpl.h"

using namespace io::openmessaging;
using namespace io::openmessaging::consumer;


StreamImpl::StreamImpl(jobject proxy) : ServiceLifecycleImpl(proxy) {
    CurrentEnv current;
    const char *klassStream = "io/openmessaging/consumer/Stream";
    classStream = current.findClass(klassStream);
    midProperties = current.getMethodId(classStream, "properties", "()Lio/openmessaging/KeyValue;");
}

StreamImpl::~StreamImpl() {
    CurrentEnv current;
    current.deleteRef(classStream);
}

boost::shared_ptr<KeyValue> StreamImpl::properties() {
    CurrentEnv current;
    jobject kv = current.callObjectMethod(_proxy, midProperties);
    boost::shared_ptr<KeyValue> pKV = boost::make_shared<KeyValueImpl>(current.env->NewGlobalRef(kv));
    current.deleteRef(kv);
    return pKV;
}

boost::shared_ptr<MessageIterator> StreamImpl::current() {

}

boost::shared_ptr<MessageIterator> StreamImpl::begin() {

}

boost::shared_ptr<MessageIterator> StreamImpl::end() {

}

boost::shared_ptr<MessageIterator> StreamImpl::seekByTime(long timestamp) {

}
