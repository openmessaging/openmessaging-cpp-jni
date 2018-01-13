#ifndef OMS_CORE_H
#define OMS_CORE_H

#include "JavaOption.h"

namespace io {
    namespace openmessaging {
        namespace core {
            void Initialize(const JavaOption& javaOption);

            void Shutdown();
        }
    }
}

#endif //OMS_CORE_H
