#ifndef OMS_PUSH_CONSUMER_IMPL_H
#define OMS_PUSH_CONSUMER_IMPL_H

#include "Namespace.h"
#include "core.h"
#include "consumer/PushConsumer.h"
#include "ServiceLifecycleImpl.h"

BEGIN_NAMESPACE_3(io, openmessaging, consumer)

    class PushConsumerImpl : public virtual consumer::PushConsumer, public virtual ServiceLifecycleImpl {
    public:
        PushConsumerImpl(jobject proxy);

        virtual ~PushConsumerImpl();

        virtual KeyValuePtr attributes();

        virtual void resume();

        virtual void suspend(long timeout);

        virtual bool isSuspended();

        virtual consumer::PushConsumer& attachQueue(const std::string &queueName,
                                                    const consumer::MessageListenerPtr &listener,
                                                    const KeyValuePtr &properties);

        virtual consumer::PushConsumer& detachQueue(const std::string &queueName);

        virtual void addInterceptor(const interceptor::ConsumerInterceptorPtr &interceptor);

        virtual void removeInterceptor(const interceptor::ConsumerInterceptorPtr &interceptor);


    private:
        jclass classPushConsumer;
        jclass classPushConsumerAdaptor;
        jclass classConsumerInterceptorAdaptor;
        jclass classMessageListenerAdaptor;

        jobject objectPushConsumerAdaptor;

        // Default constructor
        jmethodID midConsumerInterceptorAdaptor;

        // Default constructor
        jmethodID midPushConsumerAdaptor;

        jmethodID midAttributes;
        jmethodID midResume;
        jmethodID midSuspend;
        jmethodID midSuspend2;
        jmethodID midIsSuspended;
        jmethodID midAttachQueue;
        jmethodID midAttachQueue2;
        jmethodID midDetachQueue;
        jmethodID midAddInterceptor;
        jmethodID midRemoveInterceptor;
    };

END_NAMESPACE_3(io, openmessaging, core)

#endif //OMS_PUSH_CONSUMER_IMPL_H
