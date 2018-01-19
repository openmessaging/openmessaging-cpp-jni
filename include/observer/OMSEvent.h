#ifndef OMS_OMS_EVENT_H
#define OMS_OMS_EVENT_H
#include "Namespace.h"

BEGIN_NAMESPACE_3(io, openmessaging, observer)

            class OMSEvent {
            public:
                virtual ~OMSEvent() {

                }

                virtual std::string type() = 0;

                virtual std::string details() = 0;
            };

END_NAMESPACE_3(io, openmessaging, observer)
#endif //OMS_OMS_EVENT_H
