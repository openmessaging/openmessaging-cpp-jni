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
        virtual NS::shared_ptr<MessagingAccessPoint> messagingAccessPoint() = 0;

        virtual NS::shared_ptr<KeyValue> properties() = 0;

    };

END_NAMESPACE_3(io, openmessaging, interceptor)

#endif // OMS_CONSTRUCT_MESSAGING_ACCESS_POINT_CONTEXT_H
