#ifndef OMS_PUSHCONSUMERIMPL_H
#define OMS_PUSHCONSUMERIMPL_H

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

        virtual consumer::MessageIterator current();

        virtual consumer::MessageIterator begin();

        virtual consumer::MessageIterator end();

        virtual consumer::MessageIterator seekByTime(long timestamp);

    private:
        jclass classStreamingConsumer;

    };

END_NAMESPACE_3(io, openmessaging, core)

#endif //OMS_PUSHCONSUMERIMPL_H
