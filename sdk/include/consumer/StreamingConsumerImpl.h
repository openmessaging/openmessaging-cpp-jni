#ifndef OMS_PUSH_CONSUMER_IMPL_H
#define OMS_PUSH_CONSUMER_IMPL_H

#include "Namespace.h"
#include "consumer/StreamingConsumer.h"
#include "ServiceLifecycleImpl.h"
#include "consumer/MessageIterator.h"

BEGIN_NAMESPACE_3(io, openmessaging, consumer)

    class StreamingConsumerImpl : public virtual consumer::StreamingConsumer, public virtual ServiceLifecycleImpl {
    public:
        StreamingConsumerImpl(jobject proxy);

        virtual ~StreamingConsumerImpl();

        virtual boost::shared_ptr<KeyValue> properties();

        virtual std::vector<std::string> streams();

        virtual std::vector<std::string> consumers();

        virtual boost::shared_ptr<MessageIterator> current();

        virtual boost::shared_ptr<MessageIterator> begin();

        virtual boost::shared_ptr<MessageIterator> end();

        virtual boost::shared_ptr<MessageIterator> seekByTime(long timestamp);

    private:
        jclass classStreamingConsumer;

    };

END_NAMESPACE_3(io, openmessaging, core)

#endif //OMS_PUSH_CONSUMER_IMPL_H
