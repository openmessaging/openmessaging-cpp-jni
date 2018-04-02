#ifndef OMS_PRODUCER_IMPL_H
#define OMS_PRODUCER_IMPL_H

#include "producer/Producer.h"
#include "MessageFactory.h"
#include "ByteMessageImpl.h"
#include "core.h"
#include "Namespace.h"
#include "ServiceLifecycleImpl.h"

BEGIN_NAMESPACE_3(io, openmessaging, producer)

    class ProducerImpl : public virtual producer::Producer, public virtual ServiceLifecycleImpl {
    public:

        ProducerImpl(jobject proxy, const KeyValuePtr &props);

        virtual ~ProducerImpl();

        virtual KeyValuePtr attributes();

        virtual producer::SendResultPtr send(const MessagePtr &message, const KeyValuePtr &properties);

        virtual ByteMessagePtr createByteMessageToQueue(const std::string &topic, const MessageBodyPtr &body);

        virtual SendResultPtr send(const MessagePtr &message,
                                   const LocalTransactionBranchExecutorPtr &executor,
                                   const KeyValuePtr &properties);

        virtual FuturePtr sendAsync(const MessagePtr &message, const KeyValuePtr &properties);

        virtual BatchMessageSenderPtr createSequenceBatchMessageSender();

        virtual void addInterceptor(const interceptor::ProducerInterceptorPtr &interceptor);

        virtual void removeInterceptor(const interceptor::ProducerInterceptorPtr &interceptor);

    private:
        jclass     classProducer;
        jclass     classProducerAdaptor;
        jobject    objectProducerAdaptor;
        jmethodID  midCreateByteMessageToTopic;
        jmethodID  midCreateByteMessageToQueue;
        jmethodID  midStartup;
        jmethodID  midShutdown;

        // SendResult send(Message message);
        jmethodID  midSend;

        // SendResult send(Message message, KeyValue properties);
        jmethodID  midSend2;

        // SendResult send(Message message, LocalTransactionBranchExecutor branchExecutor, Object arg, KeyValue properties);
        jmethodID  midSend3;

        // ProducerAdaptor#public void sendAsync(final long opaque, Message message)
        jmethodID  midSendAsync;

        // ProducerAdaptor#public void sendAsync(final long opaque, Message message, KeyValue properties)
        jmethodID  midSendAsync2;

        // Producer#void sendOneway(Message message);
        jmethodID  midSendOneway;

        jmethodID  midSendOneway2;

        const KeyValuePtr &_properties;
    };

END_NAMESPACE_3(io, openmessaging, producer)


#endif // OMS_PRODUCER_IMPL_H
