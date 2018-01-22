#include "producer/LocalTransactionBranchExecutorImpl.h"

using namespace io::openmessaging::producer;

LocalTransactionBranchExecutorImpl::LocalTransactionBranchExecutorImpl(jobject proxy)
        : objectLocalTransactionBranchExecutor(proxy) {

}

LocalTransactionBranchExecutorImpl::~LocalTransactionBranchExecutorImpl() {
    CurrentEnv current;
    current.env->DeleteGlobalRef(objectLocalTransactionBranchExecutor);
    current.env->DeleteGlobalRef(classLocalTransactionBranchExecutor);
}

void LocalTransactionBranchExecutorImpl::executeLocalTransaction(Message &message,
                                                                 boost::shared_ptr<LocalTransactionExecutionContext> &context) {

}


void LocalTransactionBranchExecutorImpl::checkLocalTransaction(Message &message,
                                                               boost::shared_ptr<LocalTransactionCheckContext> &context) {

}

jobject LocalTransactionBranchExecutorImpl::getProxy() {
    return objectLocalTransactionBranchExecutor;
}