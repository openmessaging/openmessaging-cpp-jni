#ifndef OMS_LOCAL_TRANSACTION_BRANCH_EXECUTOR_IMPL_H
#define OMS_LOCAL_TRANSACTION_BRANCH_EXECUTOR_IMPL_H

#include "core.h"
#include "producer/LocalTransactionBranchExecutor.h"

BEGIN_NAMESPACE_3(io, openmessaging, producer)

    class LocalTransactionBranchExecutorImpl : public virtual LocalTransactionBranchExecutor {
    public:
        LocalTransactionBranchExecutorImpl(jobject proxy);

        virtual ~LocalTransactionBranchExecutorImpl();

        virtual void
        executeLocalTransaction(Message &message, boost::shared_ptr<LocalTransactionExecutionContext> &context);

        virtual void
        checkLocalTransaction(Message &message, boost::shared_ptr<LocalTransactionCheckContext> &context);

        jobject getProxy();

    private:
        jclass classLocalTransactionBranchExecutor;
        jobject objectLocalTransactionBranchExecutor;

    };

END_NAMESPACE_3(io, openmessaging, producer)


#endif //OMS_LOCAL_TRANSACTION_BRANCH_EXECUTOR_IMPL_H
