#include "ServiceLifecycleImpl.h"

using namespace io::openmessaging;

ServiceLifecycleImpl::ServiceLifecycleImpl(jobject proxy) : _proxy(proxy) {
    CurrentEnv current;
    classServiceLifecycle = current.getObjectClass(proxy);
    midStartup = current.getMethodId(classServiceLifecycle, "startup", "()V");
    midShutdown = current.getMethodId(classServiceLifecycle, "shutdown", "()V");
}

ServiceLifecycleImpl::~ServiceLifecycleImpl() {
    CurrentEnv current;
    current.deleteRef(_proxy);
    current.deleteRef(classServiceLifecycle);
};

void ServiceLifecycleImpl::startup() {
    CurrentEnv current;
    current.callVoidMethod(_proxy, midStartup);
}

void ServiceLifecycleImpl::shutdown() {
    CurrentEnv current;
    current.callVoidMethod(_proxy, midShutdown);
}