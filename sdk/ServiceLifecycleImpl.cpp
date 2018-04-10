#include "ServiceLifecycleImpl.h"

using namespace io::openmessaging;

ServiceLifecycleImpl::ServiceLifecycleImpl(jobject proxy) : _proxy(proxy) {
    CurrentEnv current;
    classServiceLifecycle = current.getObjectClass(proxy);
    midStartup = current.getMethodId(classServiceLifecycle, "startup", "()V");
    midShutdown = current.getMethodId(classServiceLifecycle, "shutdown", "()V");
}

ServiceLifecycleImpl::~ServiceLifecycleImpl() {
    CurrentEnv context;
    context.deleteRef(_proxy);
    context.deleteRef(classServiceLifecycle);
};

void ServiceLifecycleImpl::startup() {
    CurrentEnv context;
    context.callVoidMethod(_proxy, midStartup);
}

void ServiceLifecycleImpl::shutdown() {
    CurrentEnv context;
    context.callVoidMethod(_proxy, midShutdown);
}