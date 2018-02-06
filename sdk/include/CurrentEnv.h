#ifndef OMS_CURRENT_ENV_H
#define OMS_CURRENT_ENV_H

#include "Namespace.h"

BEGIN_NAMESPACE_2(io, openmessaging)
    class CurrentEnv {
    public:
        CurrentEnv();

        ~CurrentEnv();

        JNIEnv *env;

        template<typename T>
        T makeGlobal(T localRef) {
            T globalRef = static_cast<T>(env->NewGlobalRef(localRef));
            env->DeleteLocalRef(localRef);
            return boost::move(globalRef);
        }

        bool checkAndClearException();

        void deleteLocalRef(jobject localRef) {
            env->DeleteLocalRef(localRef);
        }
    private:
        bool attached;
    };

END_NAMESPACE_2(io, openmessaging)


#endif //OMS_CURRENT_ENV_H
