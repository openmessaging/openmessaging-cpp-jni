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

        virtual ByteMessagePtr createBytesMessage(const std::string &topic, const MessageBody &body);

        virtual SendResultPtr send(const MessagePtr &message,
                                   const LocalTransactionExecutorPtr &executor,
                                   const KeyValuePtr &properties);

        virtual FuturePtr sendAsync(const MessagePtr &message, const KeyValuePtr &properties);

        virtual void sendOneway(const MessagePtr &message, const KeyValuePtr &properties);

        virtual BatchMessageSenderPtr createSequenceBatchMessageSender();

        virtual void addInterceptor(const interceptor::ProducerInterceptorPtr &interceptor);

        virtual void removeInterceptor(const interceptor::ProducerInterceptorPtr &interceptor);

    private:
        jclass     classProducer;
        jclass     classProducerAdaptor;
        jclass     classProducerInterceptorAdaptor;

        jobject    objectProducerAdaptor;

        jmethodID midCreateBytesMessage;

        jmethodID midAttributes;
        jmethodID midSend;
        jmethodID midSend2;
        jmethodID midSend3;
        jmethodID midSendAsync;
        jmethodID midSendAsync2;
        jmethodID midSendOneway;
        jmethodID midSendOneway2;
        jmethodID midCreateBatchMessageSender;
        jmethodID midAddInterceptor;
        jmethodID midRemoveInterceptor;

        jmethodID midProducerInterceptorAdaptor;

        const KeyValuePtr &_properties;
    };

END_NAMESPACE_3(io, openmessaging, producer)


#endif // OMS_PRODUCER_IMPL_H
