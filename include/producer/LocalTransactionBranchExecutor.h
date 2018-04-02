#ifndef OMS_LOCAL_TRANSACTION_BRANCH_EXECUTOR_H
#define OMS_LOCAL_TRANSACTION_BRANCH_EXECUTOR_H

#include "Namespace.h"
#include "Message.h"
#include "producer/LocalTransactionExecutionContext.h"
#include "producer/LocalTransactionCheckContext.h"
#include "Uncopyable.h"

BEGIN_NAMESPACE_3(io, openmessaging, producer)

    /**
     * Each executor will be associated with a transactional message, can be used to execute local transaction
     * branch and submit the transaction status(commit or rollback).
     * <p>
     *
     * The associated message will be exposed to consumer when the local transaction has been committed, or be
     * discarded if local transaction has been rolled back.
     *
     * <p>
     * If the executor don't submit the transaction status for a long time, the server may lookup it forwardly through
     * {@link LocalTransactionBranchExecutor#checkLocalTransactionBranch(Message, CheckLocalTransactionBranchContext)}
     *
     * @version OMS 1.0
     * @since OMS 1.0
     */
    class LocalTransactionBranchExecutor : private Uncopyable {
    public:
        virtual ~LocalTransactionBranchExecutor() {

        }

        virtual void
        executeLocalTransaction(Message &message, const LocalTransactionExecutionContextPtr &context) = 0;

        virtual void
        checkLocalTransaction(Message &message, const LocalTransactionCheckContextPtr &context) = 0;

    };

    typedef NS::shared_ptr<LocalTransactionBranchExecutor> LocalTransactionBranchExecutorPtr;

END_NAMESPACE_3(io, openmessaging, producer)

#endif //OMS_LOCAL_TRANSACTION_BRANCH_EXECUTOR_H
