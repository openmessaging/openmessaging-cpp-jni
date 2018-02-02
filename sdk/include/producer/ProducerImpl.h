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

        ProducerImpl(jobject proxy, boost::shared_ptr<KeyValue> properties);

        virtual ~ProducerImpl();

        virtual boost::shared_ptr<KeyValue> properties();

        virtual boost::shared_ptr<producer::SendResult> send(boost::shared_ptr<Message> message,
                                           boost::shared_ptr<KeyValue> properties);

        virtual boost::shared_ptr<ByteMessage>
        createByteMessageToTopic(std::string &topic, std::vector<unsigned char> &body);

        virtual boost::shared_ptr<ByteMessage>
        createByteMessageToQueue(std::string &topic, std::vector<unsigned char> &body);

        virtual boost::shared_ptr<SendResult> send(boost::shared_ptr<Message> message,
                                                   boost::shared_ptr<LocalTransactionBranchExecutor> executor,
                                                   boost::shared_ptr<void> arg,
                                                   boost::shared_ptr<KeyValue> properties);

        virtual boost::shared_ptr<Future>
        sendAsync(boost::shared_ptr<Message> message,
                  boost::shared_ptr<KeyValue> properties);

        virtual void sendOneway(boost::shared_ptr<Message> message,
                                boost::shared_ptr<KeyValue> properties);

        virtual boost::shared_ptr<BatchMessageSender> createSequenceBatchMessageSender();

        virtual void addInterceptor(boost::shared_ptr<interceptor::ProducerInterceptor> interceptor);

        virtual void removeInterceptor(boost::shared_ptr<interceptor::ProducerInterceptor> interceptor);

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

        boost::shared_ptr<KeyValue> _properties;
    };

END_NAMESPACE_3(io, openmessaging, producer)


#endif // OMS_PRODUCER_IMPL_H