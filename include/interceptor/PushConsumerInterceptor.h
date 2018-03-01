#ifndef OMS_PUSH_CONSUMER_INTERCEPTOR_H
#define OMS_PUSH_CONSUMER_INTERCEPTOR_H

#include <boost/smart_ptr.hpp>
#include "Namespace.h"
#include "Uncopyable.h"

BEGIN_NAMESPACE_3(io, openmessaging, interceptor)

    class OnMessageBeforeContext : private Uncopyable {
    public:
        virtual ~OnMessageBeforeContext() {

        }
    };

    class OnMessageAfterContext : private Uncopyable {
    public:
        virtual ~OnMessageAfterContext() {

        }
    };

    class PushConsumerInterceptor : private Uncopyable {
    public:
        virtual ~PushConsumerInterceptor() {

        }

        virtual void onMessageBefore(boost::shared_ptr<OnMessageBeforeContext> context);
    };

END_NAMESPACE_3(io, openmessaging, interceptor)

#endif //OMS_PUSH_CONSUMER_INTERCEPTOR_H
