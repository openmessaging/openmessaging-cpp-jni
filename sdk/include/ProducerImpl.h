#ifndef OMS_PRODUCERIMPL_H
#define OMS_PRODUCERIMPL_H

#include "producer/Producer.h"
#include "core.h"
#include "Namespace.h"

BEGIN_NAMESPACE_3(io, openmessaging, core)

    class ProducerImpl: public producer::Producer {
    public:
        ProducerImpl(jobject proxy, boost::shared_ptr<KeyValue> properties);

        ~ProducerImpl();

        boost::shared_ptr<KeyValue> properties();

        boost::shared_ptr<producer::SendResult> send(boost::shared_ptr<Message> message,
                                           boost::shared_ptr<KeyValue> properties);

    private:
        jobject objectProducer;

        boost::shared_ptr<KeyValue> _properties;
    };

END_NAMESPACE_3(io, openmessaging, core)


#endif // PRODUCER_H
