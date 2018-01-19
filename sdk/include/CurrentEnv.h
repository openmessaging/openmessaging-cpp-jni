#ifndef OMS_CURRENT_ENV_H
#define OMS_CURRENT_ENV_H

#include "Namespace.h"

BEGIN_NAMESPACE_2(io, openmessaging)
    class CurrentEnv {
    public:
        CurrentEnv();

        ~CurrentEnv();

        JNIEnv *env;

    private:
        bool attached;
    };

END_NAMESPACE_2(io, openmessaging)


#endif //OMS_CURRENT_ENV_H
