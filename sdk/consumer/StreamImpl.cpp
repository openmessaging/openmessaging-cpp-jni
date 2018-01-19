#include "consumer/StreamImpl.h"
#include "KeyValueImpl.h"

BEGIN_NAMESPACE_3(io, openmessaging, consumer)

    using namespace io::openmessaging::consumer;

    StreamImpl::StreamImpl(jobject proxy) : ServiceLifecycleImpl(proxy) {
        CurrentEnv current;
        jclass classStreamLocal = current.env->FindClass("io/openmessaging/consumer/Stream");
        classStream = reinterpret_cast<jclass>(current.env->NewGlobalRef(classStreamLocal));
        current.env->DeleteLocalRef(classStreamLocal);
        midProperties = getMethod(current, classStream, "properties", "()Lio/openmessaging/KeyValue;");
    }

    StreamImpl::~StreamImpl() {
        CurrentEnv current;
        current.env->DeleteGlobalRef(classStream);
    }

    boost::shared_ptr<KeyValue> StreamImpl::properties() {
        CurrentEnv current;
        jobject kv = current.env->CallObjectMethod(_proxy, midProperties);
        boost::shared_ptr<KeyValue> pKV = boost::make_shared<KeyValueImpl>(current.env->NewGlobalRef(kv));
        current.env->DeleteLocalRef(kv);
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


END_NAMESPACE_3(io, openmessaging, consumer)
