#ifndef OMS_STREAMIMPL_H
#define OMS_STREAMIMPL_H

#include "consumer/Stream.h"
#include "ServiceLifecycleImpl.h"
#include "core.h"

BEGIN_NAMESPACE_3(io, openmessaging, consumer)
    class StreamImpl : public virtual consumer::Stream, public virtual ServiceLifecycleImpl {
    public:
        StreamImpl(jobject proxy);

        virtual ~StreamImpl();

        virtual boost::shared_ptr<KeyValue> properties();

        virtual boost::shared_ptr<consumer::MessageIterator> current();

        virtual boost::shared_ptr<consumer::MessageIterator> begin();

        virtual boost::shared_ptr<consumer::MessageIterator> end();

        virtual boost::shared_ptr<consumer::MessageIterator> seekByTime(long timestamp);

    private:
        jobject objectStream;

        jclass classStream;

        jmethodID midProperties;
    };
END_NAMESPACE_3(io, openmessaging, core)

#endif //OMS_STREAMIMPL_H
