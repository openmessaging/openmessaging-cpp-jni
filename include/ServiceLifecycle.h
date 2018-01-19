#ifndef OMS_SERVICE_LIFECYCLE_H
#define OMS_SERVICE_LIFECYCLE_H

#include "Namespace.h"

BEGIN_NAMESPACE_2(io, openmessaging)

        class ServiceLifecycle {
        public:
            virtual void startup() = 0;

            virtual void shutdown() = 0;

            virtual ~ServiceLifecycle() {
            }
        };

END_NAMESPACE_2(io, openmessaging)

#endif // OMS_SERVICE_LIFECYCLE_H
