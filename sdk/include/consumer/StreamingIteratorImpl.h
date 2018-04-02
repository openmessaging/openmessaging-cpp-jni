#ifndef OMS_STREAMING_ITERATOR_IMPL_H
#define OMS_STREAMING_ITERATOR_IMPL_H

#include "consumer/StreamingIterator.h"

BEGIN_NAMESPACE_3(io, openmessaging, consumer)

    class StreamingIteratorImpl : public virtual StreamingIterator {
    public:
        virtual KeyValuePtr attributes();

        virtual void commit(bool flush);

        virtual bool hasNext();

        virtual MessagePtr next();

        virtual bool hasPrevious();

        virtual MessagePtr previous();

    };
END_NAMESPACE_3(io, openmessaging, consumer)

#endif //OMS_STREAMING_ITERATOR_IMPL_H
