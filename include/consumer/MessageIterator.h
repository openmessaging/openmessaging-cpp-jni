#ifndef OMS_MESSAGEITERATOR_H
#define OMS_MESSAGEITERATOR_H

#include <boost/shared_ptr.hpp>

#include "ServiceLifecycle.h"

namespace io {
    namespace openmessaging {
        namespace consumer {
            class MessageIterator : public virtual ServiceLifecycle {
            public:
                virtual ~MessageIterator() {

                }

                virtual boost::shared_ptr<KeyValue> properties() = 0;

                virtual void commit(bool flush) = 0;

                virtual bool hasNext() = 0;

                virtual boost::shared_ptr<Message> next() = 0;

                virtual bool hasPrevious() = 0;

                virtual boost::shared_ptr<Message> previous() = 0;
            };
        }
    }
}

#endif //OMS_MESSAGEITERATOR_H
