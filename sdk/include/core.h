#ifndef OMS_CORE_H
#define OMS_CORE_H

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>

#include "JavaOption.h"

namespace io {
    namespace openmessaging {
        namespace core {
            extern JavaVM *jvm;

            void Initialize();

            void Shutdown();

            bool isRunning();

            class CurrentEnv {
            public:
                CurrentEnv();

                ~CurrentEnv();

                JNIEnv *env;

            private:
                bool attached;
            };
        }
    }
}

#endif //OMS_CORE_H
