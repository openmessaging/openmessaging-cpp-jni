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

        ProducerImpl(jobject proxy,
                     const NS::shared_ptr<KeyValue> &props);

        virtual ~ProducerImpl();

        virtual NS::shared_ptr<KeyValue> properties();

        virtual NS::shared_ptr<producer::SendResult> send(const NS::shared_ptr<Message> &message,
                                                             const NS::shared_ptr<KeyValue> &properties);

        virtual NS::shared_ptr<ByteMessage>
        createByteMessageToTopic(const std::string &topic, const scoped_array<char> &body);

        virtual NS::shared_ptr<ByteMessage>
        createByteMessageToQueue(const std::string &topic, const scoped_array<char> &body);

        virtual NS::shared_ptr<SendResult> send(const NS::shared_ptr<Message> &message,
                                                   const NS::shared_ptr<LocalTransactionBranchExecutor> &executor,
                                                   const NS::shared_ptr<void> &arg,
                                                   const NS::shared_ptr<KeyValue> &properties);

        virtual NS::shared_ptr<Future>
        sendAsync(const NS::shared_ptr<Message> &message,
                  const NS::shared_ptr<KeyValue> &properties);

        virtual void sendOneway(const NS::shared_ptr<Message> &message,
                                const NS::shared_ptr<KeyValue> &properties);

        virtual NS::shared_ptr<BatchMessageSender> createSequenceBatchMessageSender();

        virtual void addInterceptor(const NS::shared_ptr<interceptor::ProducerInterceptor> &interceptor);

        virtual void removeInterceptor(const NS::shared_ptr<interceptor::ProducerInterceptor> &interceptor);

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

        const NS::shared_ptr<KeyValue> &_properties;
    };

END_NAMESPACE_3(io, openmessaging, producer)


#endif // OMS_PRODUCER_IMPL_H
