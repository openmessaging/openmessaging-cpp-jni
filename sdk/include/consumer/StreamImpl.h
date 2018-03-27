#ifndef OMS_STREAM_IMPL_H
#define OMS_STREAM_IMPL_H

#include "consumer/Stream.h"
#include "ServiceLifecycleImpl.h"
#include "core.h"

BEGIN_NAMESPACE_3(io, openmessaging, consumer)
    class StreamImpl : public virtual consumer::Stream, public virtual ServiceLifecycleImpl {
    public:
        StreamImpl(jobject proxy);

        virtual ~StreamImpl();

        virtual NS::shared_ptr<KeyValue> properties();

        virtual NS::shared_ptr<consumer::MessageIterator> current();

        virtual NS::shared_ptr<consumer::MessageIterator> begin();

        virtual NS::shared_ptr<consumer::MessageIterator> end();

        virtual NS::shared_ptr<consumer::MessageIterator> seekByTime(long timestamp);

    private:
        jclass classStream;

        jmethodID midProperties;
    };
END_NAMESPACE_3(io, openmessaging, core)

#endif //OMS_STREAM_IMPL_H
