#ifndef OMS_PUSHCONSUMERIMPL_H
#define OMS_PUSHCONSUMERIMPL_H

#include "Namespace.h"
#include "core.h"
#include "consumer/PushConsumer.h"
#include "ServiceLifecycleImpl.h"

BEGIN_NAMESPACE_3(io, openmessaging, core)

    class PushConsumerImpl : public virtual consumer::PushConsumer, public virtual ServiceLifecycleImpl {
    public:
        PushConsumerImpl(jobject proxy);

    private:
        jobject objectPushConsumer;

    };

END_NAMESPACE_3(io, openmessaging, core)

#endif //OMS_PUSHCONSUMERIMPL_H
