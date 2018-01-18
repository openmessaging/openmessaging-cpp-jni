#ifndef OPEN_MESSAGING_H
#define OPEN_MESSAGING_H

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "KeyValue.h"
#include "Namespace.h"

BEGIN_NAMESPACE_2(io, openmessaging)

        boost::shared_ptr<KeyValue> kv_nullptr;

        class OMS {
        public:
            static boost::shared_ptr<KeyValue> newKeyValue();
        };

END_NAMESPACE_2(io, openmessaging)
#endif
