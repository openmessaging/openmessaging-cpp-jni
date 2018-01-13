#ifndef OMS_OBSERVER_H
#define OMS_OBSERVER_H

#include <boost/shared_ptr.hpp>

#include "OMSEvent.h"

namespace io {
    namespace openmessaging {
        namespace observer {
            class Observer {
            public:
                virtual ~Observer() {

                }

                virtual void onEvent(boost::shared_ptr<OMSEvent> &event) = 0;
            };
        }
    }
}

#endif //OMS_OBSERVER_H
