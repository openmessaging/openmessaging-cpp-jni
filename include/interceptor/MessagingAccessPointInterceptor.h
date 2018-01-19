#ifndef OMS_MESSAGING_ACCESS_POINT_INTERCEPTOR_H
#define OMS_MESSAGING_ACCESS_POINT_INTERCEPTOR_H

#include <boost/shared_ptr.hpp>

#include "Namespace.h"
#include "MessagingAccessPoint.h"
#include "ConstructMessagingAccessPointContext.h"

BEGIN_NAMESPACE_3(io, openmessaging, interceptor)
    class MessagingAccessPointInterceptor {
    public:
        virtual boost::shared_ptr<MessagingAccessPoint> constructMessagingAccessPoint(ConstructMessagingAccessPointContext &context) = 0;
    };
END_NAMESPACE_3(io, openmessaging, interceptor)
#endif //OMS_MESSAGING_ACCESS_POINT_INTERCEPTOR_H
