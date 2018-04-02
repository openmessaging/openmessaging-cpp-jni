#ifndef OMS_PRODUCER_INTERCEPTOR_H
#define OMS_PRODUCER_INTERCEPTOR_H

#include "Namespace.h"
#include "Uncopyable.h"
#include "smart_pointer.h"

BEGIN_NAMESPACE_3(io, openmessaging, interceptor)

    class PreSendContext : private Uncopyable {
    public:
        virtual ~PreSendContext() {
        }
    };
    typedef NS::shared_ptr<PreSendContext> PreSendContextPtr;

    class PostSendContext {
    public:
        virtual ~PostSendContext() {
        }
    };
    typedef NS::shared_ptr<PostSendContext> PostSendContextPtr;

    /**
     * A {@code ProducerInterceptor} is used to intercept send operations of producer.
     *
     * @version OMS 1.0
     * @since OMS 1.0
     */
    class ProducerInterceptor : private Uncopyable {
    public:
        virtual ~ProducerInterceptor() {

        }

        virtual void preSend(PreSendContextPtr &context);

        virtual void postSend(PostSendContextPtr &context);
    };
    typedef NS::shared_ptr<ProducerInterceptor> ProducerInterceptorPtr;

END_NAMESPACE_3(io, openmessaging, interceptor)

#endif // OMS_PRODUCER_INTERCEPTOR_H
