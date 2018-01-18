#ifndef OMS_PUSHCONSUMERINTERCEPTOR_H
#define OMS_PUSHCONSUMERINTERCEPTOR_H

#include "Namespace.h"
#include <boost/shared_ptr.hpp>

BEGIN_NAMESPACE_3(io, openmessaging, interceptor)

    class OnMessageBeforeContext {
    public:
        virtual ~OnMessageBeforeContext() {

        }
    };

    class OnMessageAfterContext {
    public:
        virtual ~OnMessageAfterContext() {

        }
    };

    class PushConsumerInterceptor {
    public:
        virtual ~PushConsumerInterceptor() {

        }

        virtual void onMessageBefore(boost::shared_ptr<OnMessageBeforeContext> context);
    };

END_NAMESPACE_3(io, openmessaging, interceptor)

#endif //OMS_PUSHCONSUMERINTERCEPTOR_H
