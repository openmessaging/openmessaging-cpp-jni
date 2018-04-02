#ifndef OMS_MESSAGING_ACCESS_POINT_INTERCEPTOR_H
#define OMS_MESSAGING_ACCESS_POINT_INTERCEPTOR_H

#include "smart_pointer.h"
#include "Namespace.h"
#include "MessagingAccessPoint.h"
#include "ConstructMessagingAccessPointContext.h"
#include "Uncopyable.h"

BEGIN_NAMESPACE_3(io, openmessaging, interceptor)
    /**
     * A {@code MessagingAccessPointInterceptor} is used to wrap {@code MessagingAccessPoint}.
     *
     * @version OMS 1.0
     * @since OMS 1.0
     */
    class MessagingAccessPointInterceptor : private Uncopyable {
    public:
        virtual MessagingAccessPointPtr constructMessagingAccessPoint(ConstructMessagingAccessPointContext &context) = 0;
    };
    typedef NS::shared_ptr<MessagingAccessPointInterceptor> MessagingAccessPointInterceptorPtr;

END_NAMESPACE_3(io, openmessaging, interceptor)
#endif //OMS_MESSAGING_ACCESS_POINT_INTERCEPTOR_H
