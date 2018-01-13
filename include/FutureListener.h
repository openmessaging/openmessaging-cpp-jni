#ifndef OMS_FUTURELISTENER_H
#define OMS_FUTURELISTENER_H

#include "Future.h"

namespace io {
    namespace openmessaging {
        class FutureListener {
        public:
            virtual FutureListener() {

            }

            virtual operationComplete(const Future &future) = 0;
        };
    }
}

#endif //OMS_FUTURELISTENER_H
