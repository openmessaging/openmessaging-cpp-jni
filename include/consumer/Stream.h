#ifndef OMS_STREAM_H
#define OMS_STREAM_H

#include <boost/shared_ptr.hpp>

#include "ServiceLifecycle.h"

namespace io {
    namespace openmessaging {
        namespace consumer {
            class Stream : public virtual ServiceLifecycle {
            public:
                virtual ~Stream() {

                }

                virtual boost::shared_ptr<KeyValue> properties() = 0;
            };
        }
    }
}

#endif //OMS_STREAM_H
