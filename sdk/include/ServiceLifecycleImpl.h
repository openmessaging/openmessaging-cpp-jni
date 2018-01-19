#ifndef OMS_SERVICE_LIFECYCLE_IMPL_H
#define OMS_SERVICE_LIFECYCLE_IMPL_H

#include "ServiceLifecycle.h"
#include "core.h"
#include "Namespace.h"

BEGIN_NAMESPACE_2(io, openmessaging)

    class ServiceLifecycleImpl : public virtual ServiceLifecycle {
    public:
        ServiceLifecycleImpl(jobject proxy);

        virtual ~ServiceLifecycleImpl();

        virtual void startup();

        virtual void shutdown();

    protected:
        jobject _proxy;

    private:
        jclass classServiceLifecycle;
        jmethodID  midStartup;
        jmethodID  midShutdown;
    };

END_NAMESPACE_2(io, openmessaging)

#endif //OMS_SERVICE_LIFECYCLE_IMPL_H
