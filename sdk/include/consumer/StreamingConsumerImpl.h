#ifndef OMS_PUSH_CONSUMER_IMPL_H
#define OMS_PUSH_CONSUMER_IMPL_H

#include "Namespace.h"
#include "consumer/StreamingConsumer.h"
#include "ServiceLifecycleImpl.h"
#include "consumer/StreamingIterator.h"

BEGIN_NAMESPACE_3(io, openmessaging, consumer)

    class StreamingConsumerImpl : public virtual consumer::StreamingConsumer, public virtual ServiceLifecycleImpl {
    public:
        StreamingConsumerImpl(jobject proxy);

        virtual ~StreamingConsumerImpl();

        virtual KeyValuePtr attributes();

        virtual StreamingIteratorPtr seek(const std::string &name, long offset, int whence);

    private:
        jclass classStreamingConsumer;

    };

END_NAMESPACE_3(io, openmessaging, core)

#endif //OMS_PUSH_CONSUMER_IMPL_H
