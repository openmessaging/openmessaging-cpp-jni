#ifndef OMS_PROMISE_H
#define OMS_PROMISE_H

#include <boost/shared_ptr.hpp>

#include "Future.h"
#include "producer/SendResult.h"

namespace io {
    namespace openmessaging {
        class Promise : public Future {
        public:
            bool cancel(bool interruptIfRunning) = 0;

            bool set(boost::shared_mutex<producer::SendResult> &value) = 0;

            bool setFailure(std::exception& e) = 0;

        protected:
            virtual ~Promise() {
            }
        };
    }
}

#endif //OMS_PROMISE_H
