#include "PushConsumerImpl.h"

BEGIN_NAMESPACE_3(io, openmessaging, core)

    PushConsumerImpl::PushConsumerImpl(jobject proxy) : objectPushConsumer(proxy), ServiceLifecycleImpl(proxy) {

    }

END_NAMESPACE_3(io, openmessaging, core)