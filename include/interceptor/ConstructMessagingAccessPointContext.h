#ifndef OMS_CONSTRUCTMESSAGINGACCESSPOINTCONTEXT_H
#define OMS_CONSTRUCTMESSAGINGACCESSPOINTCONTEXT_H

#include <boost/shared_ptr.hpp>

#include "Namespace.h"
#include "KeyValue.h"
#include "MessagingAccessPoint.h"

BEGIN_NAMESPACE_3(io, openmessaging, interceptor)

    class ConstructMessagingAccessPointContext {
    public:
        virtual boost::shared_ptr<MessagingAccessPoint> messagingAccessPoint() = 0;

        virtual boost::shared_ptr<KeyValue> properties() = 0;

    };

END_NAMESPACE_3(io, openmessaging, interceptor)

#endif //OMS_CONSTRUCTMESSAGINGACCESSPOINTCONTEXT_H
