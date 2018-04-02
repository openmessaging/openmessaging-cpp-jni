#ifndef OMS_CONSTRUCT_MESSAGING_ACCESS_POINT_CONTEXT_H
#define OMS_CONSTRUCT_MESSAGING_ACCESS_POINT_CONTEXT_H

#include "smart_pointer.h"
#include "Namespace.h"
#include "KeyValue.h"
#include "MessagingAccessPoint.h"
#include "Uncopyable.h"

BEGIN_NAMESPACE_3(io, openmessaging, interceptor)

    class ConstructMessagingAccessPointContext : private Uncopyable {
    public:
        virtual MessagingAccessPointPtr messagingAccessPoint() = 0;

        virtual KeyValuePtr properties() = 0;

    };
    typedef NS::shared_ptr<ConstructMessagingAccessPointContext> ConstructMessagingAccessPointContextPtr;

END_NAMESPACE_3(io, openmessaging, interceptor)

#endif // OMS_CONSTRUCT_MESSAGING_ACCESS_POINT_CONTEXT_H
