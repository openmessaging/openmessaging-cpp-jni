#include <exception>
#include <jni.h>
#include "core.h"

BEGIN_NAMESPACE_2(io, openmessaging)
    extern JavaVM *jvm;
END_NAMESPACE_2(io, openmessaging)

using namespace io::openmessaging;

CurrentEnv::CurrentEnv() : attached(false) {
    if (jvm->GetEnv(reinterpret_cast<void **>(&(this->env)), JNI_VERSION_1_8) != JNI_OK) {
        if (jvm->AttachCurrentThread(reinterpret_cast<void **>(&(this->env)), NULL) == JNI_OK) {
            attached = true;
        }
    }
    this->env->EnsureLocalCapacity(1024);
}

CurrentEnv::~CurrentEnv() {
    if (attached) {
        jvm->DetachCurrentThread();
    }
}

bool CurrentEnv::checkAndClearException() {
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        return true;
    }

    return false;
}


jobject CurrentEnv::callObjectMethod(jobject obj, jmethodID mid, ...) {
    jobject result;
    va_list args;
    va_start(args, mid);
    result = env->CallObjectMethodV(obj, mid, args);
    va_end(args);

    if (checkAndClearException()) {
        const char* msg = "Exception raised while call Java Methods";
        BOOST_LOG_TRIVIAL(error) << msg;
        throw std::runtime_error(msg);
    }

    return result;
}