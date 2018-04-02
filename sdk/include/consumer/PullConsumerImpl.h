#ifndef OMS_PULL_CONSUMER_IMPL_H
#define OMS_PULL_CONSUMER_IMPL_H

#include "Namespace.h"
#include "core.h"
#include "ServiceLifecycleImpl.h"
#include "consumer/PullConsumer.h"

BEGIN_NAMESPACE_3(io, openmessaging, consumer)

    class PullConsumerImpl : public virtual consumer::PullConsumer, public virtual ServiceLifecycleImpl {
    public:
        PullConsumerImpl(jobject proxy);

        virtual ~PullConsumerImpl();

        virtual KeyValuePtr attributes();

        PullConsumer& attachQueue(const std::string &queueName, const KeyValuePtr &properties);

        PullConsumer& detachQueue(const std::string &queueName);

        virtual MessagePtr receive(const KeyValuePtr &props);

        virtual void ack(const std::string &messageId, const KeyValuePtr &props);

        jobject getProxy();

    private:
        jclass classPullConsumer;
        jmethodID midProperties;
        jmethodID midPoll;
        jmethodID midPoll2;
        jmethodID midAck;
        jmethodID midAck2;

    };

END_NAMESPACE_3(io, openmessaging, core)

#endif //OMS_PULL_CONSUMER_IMPL_H
