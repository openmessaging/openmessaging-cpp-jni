#ifndef OMS_CONSUMER_INTERCEPTOR_H
#define OMS_CONSUMER_INTERCEPTOR_H

#include "smart_pointer.h"
#include "Namespace.h"
#include "Uncopyable.h"

BEGIN_NAMESPACE_3(io, openmessaging, interceptor)

    class OnMessageBeforeContext : private Uncopyable {
    public:
        virtual ~OnMessageBeforeContext() {

        }
    };
    typedef NS::shared_ptr<OnMessageBeforeContext> OnMessageBeforeContextPtr;

    class OnMessageAfterContext : private Uncopyable {
    public:
        virtual ~OnMessageAfterContext() {

        }
    };
    typedef NS::shared_ptr<OnMessageAfterContext> OnMessageAfterContextPtr;

    /**
     * A {@code PushConsumerInterceptor} is used to intercept consume operations of push consumer.
     *
     * @version OMS 1.0
     * @since OMS 1.0
     */
    class ConsumerInterceptor : private Uncopyable {
    public:
        virtual ~ConsumerInterceptor() {

        }

        virtual void onMessageBefore(OnMessageBeforeContextPtr &context);
    };

    NS::shared_ptr<ConsumerInterceptor> ConsumerInterceptorPtr;

END_NAMESPACE_3(io, openmessaging, interceptor)

#endif //OMS_CONSUMER_INTERCEPTOR_H
