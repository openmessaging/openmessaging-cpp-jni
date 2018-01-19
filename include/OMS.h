#ifndef OMS_OMS_H
#define OMS_OMS_H

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "KeyValue.h"
#include "Namespace.h"

BEGIN_NAMESPACE_2(io, openmessaging)

    extern boost::shared_ptr<KeyValue> kv_nullptr;

    class OMS {
    public:
        static boost::shared_ptr<KeyValue> newKeyValue();
    };

END_NAMESPACE_2(io, openmessaging)

#endif // OMS_OMS_H
