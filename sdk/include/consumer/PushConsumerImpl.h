#ifndef OMS_PUSHCONSUMERIMPL_H
#define OMS_PUSHCONSUMERIMPL_H

#include "Namespace.h"
#include "core.h"
#include "consumer/PushConsumer.h"
#include "ServiceLifecycleImpl.h"

BEGIN_NAMESPACE_3(io, openmessaging, consumer)

    class PushConsumerImpl : public virtual consumer::PushConsumer, public virtual ServiceLifecycleImpl {
    public:
        PushConsumerImpl(jobject proxy);

        virtual ~PushConsumerImpl();

        virtual boost::shared_ptr<KeyValue> properties();

        virtual void resume();

        virtual void suspend();

        virtual bool isSuspended();

        virtual consumer::PushConsumer& attachQueue(std::string &queueName,
                                          boost::shared_ptr<consumer::MessageListener> listener,
                                          boost::shared_ptr<KeyValue> properties);

        virtual consumer::PushConsumer& detachQueue(std::string &queueName);

        virtual void addInterceptor(boost::shared_ptr<consumer::PushConsumerInterceptor> interceptor);

        virtual void removeInterceptor(boost::shared_ptr<consumer::PushConsumerInterceptor> interceptor);


    private:
        jclass classPushConsumer;

    };

END_NAMESPACE_3(io, openmessaging, core)

#endif //OMS_PUSHCONSUMERIMPL_H
