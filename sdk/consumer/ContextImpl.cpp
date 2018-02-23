#include <boost/smart_ptr.hpp>
#include "KeyValueImpl.h"
#include "consumer/ContextImpl.h"


using namespace io::openmessaging;
using namespace io::openmessaging::consumer;

ContextImpl::ContextImpl(jobject proxy) : _proxy(proxy) {
    CurrentEnv current;
    jclass classContextLocal = current.env->FindClass("io/openmessaging/consumer/Context");
    classContext = current.makeGlobal(classContextLocal);

    midProperties = current.env->GetMethodID(classContext, "properties", "()Lio/openmessaging/KeyValue;");
    midAck = current.env->GetMethodID(classContext, "ack", "()V");
    midAck2 = current.env->GetMethodID(classContext, "ack", "(Lio/openmessaging/KeyValue;)V");
}

boost::shared_ptr<KeyValue> ContextImpl::properties() {
    CurrentEnv current;
    jobject jPropertiesLocal = current.env->CallObjectMethod(_proxy, midProperties);
    if (current.checkAndClearException()) {
        abort();
    }
    jobject jProps = current.makeGlobal(jPropertiesLocal);
    return boost::make_shared<KeyValueImpl>(jProps);
}

void ContextImpl::ack(boost::shared_ptr<KeyValue> properties) {
    CurrentEnv current;
    if (properties) {
        boost::shared_ptr<KeyValueImpl> kvImpl = boost::dynamic_pointer_cast<KeyValueImpl>(properties);
        current.env->CallVoidMethod(_proxy, midAck2, kvImpl->getProxy());
    } else {
        current.env->CallVoidMethod(_proxy, midAck);
    }

    current.checkAndClearException();
}
