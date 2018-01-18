#ifndef OMS_CORE_H
#define OMS_CORE_H

#include <set>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>

#include "JavaOption.h"
#include "Namespace.h"

BEGIN_NAMESPACE_2(io, openmessaging)
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

    std::set<std::string> toNativeSet(CurrentEnv &env, jobject s);

    jmethodID getMethod(CurrentEnv &current, jclass clazz, const std::string &name, const std::string &signature,
                        bool isStatic = false);

END_NAMESPACE_2(io, openmessaging)

#endif //OMS_CORE_H
