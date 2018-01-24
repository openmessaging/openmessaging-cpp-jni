#ifndef OMS_FUTURE_LISTENER_H
#define OMS_FUTURE_LISTENER_H

#include <boost/shared_ptr.hpp>

#include "Namespace.h"

BEGIN_NAMESPACE_2(io, openmessaging)

    class Future;

    class FutureListener {
    public:
        virtual ~FutureListener() {

        }

        virtual void operationComplete(const Future& future) = 0;
    };

END_NAMESPACE_2(io, openmessaging)

#endif //OMS_FUTURE_LISTENER_H
