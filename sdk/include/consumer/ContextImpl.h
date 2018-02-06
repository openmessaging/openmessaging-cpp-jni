#ifndef OMS_CONTEXT_IMPL_H
#define OMS_CONTEXT_IMPL_H

#include "Namespace.h"
#include "consumer/Context.h"
#include "core.h"

BEGIN_NAMESPACE_3(io, openmessaging, consumer)
    class ContextImpl : public Context {
    public:

        ContextImpl(jobject proxy) : _proxy(proxy) {
            CurrentEnv current;
            jclass classContextLocal = current.env->FindClass("io/openmessaging/consumer/Context");
            classContext = current.makeGlobal(classContextLocal);
        }

        virtual boost::shared_ptr<KeyValue> properties();

        virtual void ack(boost::shared_ptr<KeyValue> properties);

    private:
        jobject _proxy;
        jclass classContext;
        jmethodID midProperties;
        jmethodID midAck;

    };
END_NAMESPACE_3(io, openmessaging, consumer)

#endif //OMS_CONTEXT_IMPL_H
