#ifndef OPEN_MESSAGING_H
#define OPEN_MESSAGING_H

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "KeyValue.h"

namespace io {
    namespace openmessaging {

        boost::shared_ptr<KeyValue> kv_nullptr;

        class OMS {
        public:
            static boost::shared_ptr<KeyValue> newKeyValue() {
                boost::shared_ptr<KeyValue> ptr = boost::make_shared<KeyValue>();
                return ptr;
            }
        };

    } // end of namespace openmessaging
} // end of namespace io


#endif
