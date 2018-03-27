#include "core.h"
#include "KeyValueImpl.h"
#include "consumer/ContextImpl.h"


using namespace io::openmessaging;
using namespace io::openmessaging::consumer;

ContextImpl::ContextImpl(jobject proxy) : _proxy(proxy) {
    CurrentEnv current;

    const char *klassContext = "io/openmessaging/consumer/Context";
    classContext = current.findClass(klassContext);

    midProperties = current.getMethodId(classContext, "properties", buildSignature(Types::KeyValue_, 0));
    midAck = current.getMethodId(classContext, "ack", buildSignature(Types::void_, 0));
    midAck2 = current.getMethodId(classContext, "ack", buildSignature(Types::void_, 1, Types::KeyValue_));
}

NS::shared_ptr<KeyValue> ContextImpl::properties() {
    CurrentEnv current;
    jobject jPropertiesLocal = current.callObjectMethod(_proxy, midProperties);
    jobject jProps = current.newGlobalRef(jPropertiesLocal);
    return NS::make_shared<KeyValueImpl>(jProps);
}

void ContextImpl::ack(NS::shared_ptr<KeyValue> props) {
    CurrentEnv current;
    if (props) {
        NS::shared_ptr<KeyValueImpl> kvImpl = NS::dynamic_pointer_cast<KeyValueImpl>(props);
        current.callVoidMethod(_proxy, midAck2, kvImpl->getProxy());
    } else {
        current.callVoidMethod(_proxy, midAck);
    }
}
