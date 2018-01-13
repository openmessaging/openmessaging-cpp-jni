#ifndef OMS_CONTEXT_H
#define OMS_CONTEXT_H

#include <boost/shared_ptr.hpp>

#include "KeyValue.h"

namespace io {
    namespace openmessaging {

        extern boost::shared_ptr<KeyValue> kv_nullptr;

        namespace consumer {
            class Context {
            public:
                virtual ~Context() {
                }

                virtual boost::shared_ptr <KeyValue> properties() = 0;

                virtual void ack(boost::shared_ptr <KeyValue> properties = kv_nullptr) = 0;
            };
        }
    }
}

#endif //OMS_CONTEXT_H
