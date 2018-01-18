#ifndef OMS_PUSHCONSUMERIMPL_H
#define OMS_PUSHCONSUMERIMPL_H

#include "Namespace.h"
#include "core.h"
#include "ServiceLifecycleImpl.h"
#include "consumer/PullConsumer.h"

BEGIN_NAMESPACE_3(io, openmessaging, core)

    class PullConsumerImpl : public virtual consumer::PullConsumer, public virtual ServiceLifecycleImpl {
    public:
        PullConsumerImpl(jobject proxy);

        virtual ~PullConsumerImpl();

        virtual boost::shared_ptr<KeyValue> properties();

        virtual boost::shared_ptr<Message> poll(boost::shared_ptr<KeyValue> properties);

        virtual void ack(const std::string &messageId,
                         boost::shared_ptr<KeyValue> properties);

        jobject getProxy();

    private:
        jobject objectPullConsumer;
        jclass classPullConsumer;
        jmethodID midProperties;
        jmethodID midPoll;
        jmethodID midPoll2;
        jmethodID midAck;
        jmethodID midAck2;

    };

END_NAMESPACE_3(io, openmessaging, core)

#endif //OMS_PUSHCONSUMERIMPL_H
