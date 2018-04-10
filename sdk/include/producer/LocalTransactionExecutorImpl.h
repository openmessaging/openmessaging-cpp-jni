#ifndef OMS_LOCAL_TRANSACTION_BRANCH_EXECUTOR_IMPL_H
#define OMS_LOCAL_TRANSACTION_BRANCH_EXECUTOR_IMPL_H

#include "core.h"
#include "producer/LocalTransactionExecutor.h"

BEGIN_NAMESPACE_3(io, openmessaging, producer)

    class LocalTransactionExecutorImpl : public virtual LocalTransactionExecutor {
    public:
        LocalTransactionExecutorImpl(jobject proxy);

        virtual ~LocalTransactionExecutorImpl();

        virtual void execute(const MessagePtr &message, const ExecutionContextPtr &context);

        virtual void check(const MessagePtr &message, const CheckContextPtr &context);

        jobject getProxy();

    private:
        jclass classLocalTransactionExecutor;
        jobject objectLocalTransactionExecutor;

    };

END_NAMESPACE_3(io, openmessaging, producer)


#endif //OMS_LOCAL_TRANSACTION_BRANCH_EXECUTOR_IMPL_H
