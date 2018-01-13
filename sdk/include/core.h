#ifndef OMS_CORE_H
#define OMS_CORE_H

#include <boost/shared_ptr.hpp>

#include "JavaOption.h"

namespace io {
    namespace openmessaging {
        namespace core {
            extern JavaVM *jvm;

            void Initialize(const boost::shared_ptr<JavaOption> &javaOption);

            void Shutdown();

            bool isRunning() {
                return jvm != NULL;
            }
        }
    }
}

#endif //OMS_CORE_H
