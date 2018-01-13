#ifndef OMS_OMSEXCEPTIONEVENT_H
#define OMS_OMSEXCEPTIONEVENT_H

#include "OMSEvent.h"

namespace io {
    namespace openmessaging {
        namespace observer {

            class OMSExceptionEvent : public virtual OMSEvent {
            public:
                virtual ~OMSExceptionEvent() {

                }
            };

        }
    }
}
#endif //OMS_OMSEXCEPTIONEVENT_H
