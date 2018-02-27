#include <boost/smart_ptr.hpp>
#include "KeyValueImpl.h"
#include "consumer/ContextImpl.h"


using namespace io::openmessaging;
using namespace io::openmessaging::consumer;

ContextImpl::ContextImpl(jobject proxy) : _proxy(proxy) {
    CurrentEnv current;

    const char *klassContext = "io/openmessaging/consumer/Context";
    classContext = current.findClass(klassContext);

    midProperties = current.getMethodId(classContext, "properties", "()Lio/openmessaging/KeyValue;");
    midAck = current.getMethodId(classContext, "ack", "()V");
    midAck2 = current.getMethodId(classContext, "ack", "(Lio/openmessaging/KeyValue;)V");
}

boost::shared_ptr<KeyValue> ContextImpl::properties() {
    CurrentEnv current;
    jobject jPropertiesLocal = current.callObjectMethod(_proxy, midProperties);
    jobject jProps = current.newGlobalRef(jPropertiesLocal);
    return boost::make_shared<KeyValueImpl>(jProps);
}

void ContextImpl::ack(boost::shared_ptr<KeyValue> properties) {
    CurrentEnv current;
    if (properties) {
        boost::shared_ptr<KeyValueImpl> kvImpl = boost::dynamic_pointer_cast<KeyValueImpl>(properties);
        current.callVoidMethod(_proxy, midAck2, kvImpl->getProxy());
    } else {
        current.callVoidMethod(_proxy, midAck);
    }
}
