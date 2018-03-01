#ifndef OMS_LOCAL_TRANSACTION_BRANCH_EXECUTOR_H
#define OMS_LOCAL_TRANSACTION_BRANCH_EXECUTOR_H

#include "Namespace.h"
#include "Message.h"
#include "producer/LocalTransactionExecutionContext.h"
#include "producer/LocalTransactionCheckContext.h"
#include "Uncopyable.h"

BEGIN_NAMESPACE_3(io, openmessaging, producer)

    class LocalTransactionBranchExecutor : private Uncopyable {
    public:
        virtual ~LocalTransactionBranchExecutor() {

        }

        virtual void
        executeLocalTransaction(Message &message, boost::shared_ptr<LocalTransactionExecutionContext> &context) = 0;

        virtual void
        checkLocalTransaction(Message &message, boost::shared_ptr<LocalTransactionCheckContext> &context) = 0;

    };
END_NAMESPACE_3(io, openmessaging, producer)

#endif //OMS_LOCAL_TRANSACTION_BRANCH_EXECUTOR_H
