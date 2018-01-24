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