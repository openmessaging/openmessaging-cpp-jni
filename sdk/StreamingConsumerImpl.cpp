#include "StreamingConsumerImpl.h"

BEGIN_NAMESPACE_3(io, openmessaging, core)
    StreamingConsumerImpl::StreamingConsumerImpl(jobject proxy) : objectStreamingConsumer(proxy),
                                                                  ServiceLifecycleImpl(proxy) {

    }
END_NAMESPACE_3(io, openmessaging, core)