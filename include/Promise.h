#ifndef OMS_PROMISE_H
#define OMS_PROMISE_H

#include <boost/shared_ptr.hpp>

#include "Future.h"
#include "producer/SendResult.h"
#include "Namespace.h"

BEGIN_NAMESPACE_2(io, openmessaging)

    class Promise : public Future {
    public:
        virtual ~Promise() {
        }

        virtual bool cancel(bool interruptIfRunning = false) = 0;

        virtual bool set(boost::shared_ptr<producer::SendResult> &value) = 0;

        virtual bool setFailure(std::exception &e) = 0;
    };

END_NAMESPACE_2(io, openmessaging)

#endif //OMS_PROMISE_H
