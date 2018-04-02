#ifndef OMS_STREAM_H
#define OMS_STREAM_H

#include "smart_pointer.h"
#include "ServiceLifecycle.h"
#include "Namespace.h"
#include "KeyValue.h"
#include "MessageIterator.h"

BEGIN_NAMESPACE_3(io, openmessaging, consumer)
    /**
     * A {@code Queue} is divided by many streams.
     * <p>
     * A {@code Stream} object supports consume messages from a
     * specified partition like a iterator.
     *
     * @version OMS 1.0
     * @see StreamingConsumer#stream(String)
     * @since OMS 1.0
     */
    class Stream : public virtual ServiceLifecycle {
    public:
        virtual ~Stream() {

        }

        virtual KeyValuePtr properties() = 0;

        virtual MessageIteratorPtr current() = 0;

        virtual MessageIteratorPtr begin() = 0;

        virtual MessageIteratorPtr end() = 0;

        virtual MessageIteratorPtr seekByTime(long timestamp) = 0;
    };
    typedef NS::shared_ptr<Stream> StreamPtr;

END_NAMESPACE_3(io, openmessaging, consumer)
#endif //OMS_STREAM_H
