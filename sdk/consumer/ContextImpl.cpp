#include "core.h"
#include "KeyValueImpl.h"
#include "consumer/ContextImpl.h"


using namespace io::openmessaging;
using namespace io::openmessaging::consumer;

ContextImpl::ContextImpl(jobject proxy) : _proxy(proxy) {
    CurrentEnv current;

    const char *klassContext = "io/openmessaging/consumer/MessageListener$Context";
    classContext = current.findClass(klassContext);

    midAttributes = current.getMethodId(classContext, "attributes", buildSignature(Types::KeyValue_, 0));
    midAck = current.getMethodId(classContext, "ack", buildSignature(Types::void_, 0));
}

KeyValuePtr ContextImpl::attributes() {
    CurrentEnv current;
    jobject jPropertiesLocal = current.callObjectMethod(_proxy, midAttributes);
    jobject jProps = current.newGlobalRef(jPropertiesLocal);
    return NS::make_shared<KeyValueImpl>(jProps);
}

void ContextImpl::ack() {
    CurrentEnv current;
    current.callVoidMethod(_proxy, midAck);
}
