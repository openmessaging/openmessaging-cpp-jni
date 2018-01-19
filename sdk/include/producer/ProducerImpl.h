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
        createByteMessageToTopic(std::string &topic, std::vector<char> &body);

        virtual boost::shared_ptr<ByteMessage>
        createByteMessageToQueue(std::string &topic, std::vector<char> &body);

    private:
        jclass classProducer;
        jmethodID  midCreateByteMessageToTopic;
        jmethodID  midCreateByteMessageToQueue;
        jmethodID  midStartup;
        jmethodID  midShutdown;
        jmethodID  midSend;
        jmethodID  midSend2;

        boost::shared_ptr<KeyValue> _properties;
    };

END_NAMESPACE_3(io, openmessaging, producer)


#endif // OMS_PRODUCER_IMPL_H
