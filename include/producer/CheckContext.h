#ifndef OMS_LOCAL_TRANSACTION_CHECK_CONTEXT_H
#define OMS_LOCAL_TRANSACTION_CHECK_CONTEXT_H

#include "Namespace.h"
#include "Uncopyable.h"

BEGIN_NAMESPACE_3(io, openmessaging, producer)

    class CheckContext : private Uncopyable {
    public:
        virtual ~CheckContext() {

        }

        virtual void commit() = 0;

        virtual void rollback() = 0;

    };

    typedef NS::shared_ptr<CheckContext> CheckContextPtr;

END_NAMESPACE_3(io, openmessaging, producer)

#endif //OMS_LOCAL_TRANSACTION_CHECK_CONTEXT_H
