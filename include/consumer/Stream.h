#ifndef OMS_STREAM_H
#define OMS_STREAM_H

#include <boost/shared_ptr.hpp>

#include "ServiceLifecycle.h"
#include "Namespace.h"
#include "KeyValue.h"
#include "MessageIterator.h"

BEGIN_NAMESPACE_3(io, openmessaging, consumer)

    class Stream : public virtual ServiceLifecycle {
    public:
        virtual ~Stream() {

        }

        virtual boost::shared_ptr<KeyValue> properties() = 0;

        virtual boost::shared_ptr<MessageIterator> current() = 0;

        virtual boost::shared_ptr<MessageIterator> begin() = 0;

        virtual boost::shared_ptr<MessageIterator> end() = 0;

        virtual boost::shared_ptr<MessageIterator> seekByTime(long timestamp) = 0;
    };

END_NAMESPACE_3(io, openmessaging, consumer)
#endif //OMS_STREAM_H
