#ifndef OMS_PRODUCERINTERCEPTOR_H
#define OMS_PRODUCERINTERCEPTOR_H

#include "Namespace.h"
BEGIN_NAMESPACE_3(io, openmessaging, interceptor)

    class PreSendContext {
    public:
        virtual ~PreSendContext() {
        }
    };

    class PostSendContext {
    public:
        virtual ~PostSendContext() {
        }
    };

    class ProducerInterceptor {
    public:
        virtual ~ProducerInterceptor() {

        }

        virtual void preSend(boost::shared_ptr<PreSendContext> context);

        virtual void postSend(boost::shared_ptr<PostSendContext> context);
    };

END_NAMESPACE_3(io, openmessaging, interceptor)

#endif //OMS_PRODUCERINTERCEPTOR_H
