#include "producer/LocalTransactionExecutorImpl.h"
#include "ByteMessageImpl.h"

using namespace io::openmessaging::producer;

LocalTransactionExecutorImpl::LocalTransactionExecutorImpl(jobject proxy)
        : objectLocalTransactionExecutor(proxy) {

}

LocalTransactionExecutorImpl::~LocalTransactionExecutorImpl() {
    CurrentEnv current;
    current.deleteRef(objectLocalTransactionExecutor);
    current.deleteRef(classLocalTransactionExecutor);
}

void LocalTransactionExecutorImpl::execute(const MessagePtr &message, const ExecutionContextPtr &context) {

}


void LocalTransactionExecutorImpl::check(const MessagePtr &message, const CheckContextPtr &context) {

}

jobject LocalTransactionExecutorImpl::getProxy() {
    return objectLocalTransactionExecutor;
}