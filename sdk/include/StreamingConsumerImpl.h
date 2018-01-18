#ifndef OMS_PUSHCONSUMERIMPL_H
#define OMS_PUSHCONSUMERIMPL_H

#include "Namespace.h"
#include "consumer/StreamingConsumer.h"
#include "ServiceLifecycleImpl.h"

BEGIN_NAMESPACE_3(io, openmessaging, core)

    class StreamingConsumerImpl : public virtual consumer::StreamingConsumer, public virtual ServiceLifecycleImpl {
    public:
        StreamingConsumerImpl(jobject proxy);

    private:
        jobject objectStreamingConsumer;

    };

END_NAMESPACE_3(io, openmessaging, core)

#endif //OMS_PUSHCONSUMERIMPL_H
