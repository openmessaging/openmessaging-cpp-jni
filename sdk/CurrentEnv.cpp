#include <jni.h>

#include "core.h"
#include "OMSException.h"

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

CurrentEnv::CurrentEnv(JNIEnv *ctx) : env(ctx), attached(false) {

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
        const char* msg = "Exception raised while call Java Methods: callObjectMethod";
        LOG_ERROR << msg;
        throw OMSException(msg);
    }

    return result;
}

jobject CurrentEnv::callStaticObjectMethod(jclass clazz, jmethodID mid, ...) {
    jobject result;
    va_list args;
    va_start(args, mid);
    result = env->CallStaticObjectMethodV(clazz, mid, args);
    va_end(args);

    if (checkAndClearException()) {
        const char* msg = "Exception raised while call Java Methods: callObjectMethod";
        LOG_ERROR << msg;
        throw OMSException(msg);
    }

    return result;
}

void CurrentEnv::callVoidMethod(jobject obj, jmethodID mid, ...) {
    va_list args;
    va_start(args, mid);
    env->CallVoidMethodV(obj, mid, args);
    va_end(args);

    if (checkAndClearException()) {
        const char *msg = "Exception raised while call Java Method: callVoidMethod";
        LOG_ERROR << msg;
        throw OMSException(msg);
    }
}

void CurrentEnv::callStaticVoidMethod(jclass clazz, jmethodID mid, ...) {
    va_list args;
    va_start(args, mid);
    env->CallStaticVoidMethodV(clazz, mid, args);
    va_end(args);

    if (checkAndClearException()) {
        const char *msg = "Exception raised while call Java Method: callVoidMethod";
        LOG_ERROR << msg;
        throw OMSException(msg);
    }
}

bool CurrentEnv::callBooleanMethod(jobject obj, jmethodID mid, ...) {
    va_list args;
    va_start(args, mid);
    bool result = env->CallBooleanMethodV(obj, mid, args);
    va_end(args);

    if (checkAndClearException()) {
        throw OMSException("Exception raised while calling Java Method: callBooleanMethod");
    }

    return result;
}

int CurrentEnv::callIntMethod(jobject obj, jmethodID mid, ...) {
    va_list args;
    va_start(args, mid);
    int result = env->CallIntMethodV(obj, mid, args);
    va_end(args);

    if (checkAndClearException()) {
        throw OMSException("Exception raised while calling Java Method: callBooleanMethod");
    }

    return result;
}

long CurrentEnv::callLongMethod(jobject obj, jmethodID mid, ...) {
    va_list args;
    va_start(args, mid);
    long result = env->CallLongMethodV(obj, mid, args);
    va_end(args);

    if (checkAndClearException()) {
        throw OMSException("Exception raised while calling Java Method: callBooleanMethod");
    }

    return result;
}


double CurrentEnv::callDoubleMethod(jobject obj, jmethodID mid, ...) {
    va_list args;
    va_start(args, mid);
    double result = env->CallDoubleMethodV(obj, mid, args);
    va_end(args);

    if (checkAndClearException()) {
        throw OMSException("Exception raised while calling Java Method: callBooleanMethod");
    }

    return result;
}

jclass CurrentEnv::findClass(const char *clazz) {
    jclass result = env->FindClass(clazz);
    if (NULL == result) {
        std::string msg = "Class: ";
        msg += clazz;
        msg += " is not found in classpath";
        throw OMSException(msg);
    }
    return newGlobalRef(result);
}

jclass CurrentEnv::getObjectClass(jobject obj) {
    if (env->IsSameObject(NULL, obj)) {
        throw OMSException("object is null");
    }

    jclass clazz = env->GetObjectClass(obj);
    if (checkAndClearException()) {
        throw OMSException("Fail to execute getObjectClass");
    }

    return newGlobalRef(clazz);
}

jmethodID CurrentEnv::getMethodId(jclass klass, const char *method, const std::string &sig, bool is_static) {
    return getMethodId(klass, method, sig.c_str(), is_static);
}

jmethodID CurrentEnv::getMethodId(jclass klass, const char *method, const char* sig, bool is_static) {
    jmethodID mid = NULL;
    if (is_static) {
        mid = env->GetStaticMethodID(klass, method, sig);
    } else {
        mid = env->GetMethodID(klass, method, sig);
    }

    if (NULL == mid) {
        LOG_ERROR << "Method[" << method << "], signature: [" << sig << "] is not found";
        throw OMSException("Method is not found");
    }

    return mid;
}

jobject CurrentEnv::newObject(jclass klass, jmethodID mid, ...) {
    va_list args;
    va_start(args, mid);
    jobject result = env->NewObjectV(klass, mid, args);
    va_end(args);
    if (NULL == result) {
        LOG_ERROR << "Fail to new object";
        throw OMSException("Fail to create new object");
    }

    checkAndClearException();

    return newGlobalRef(result);
}


jstring CurrentEnv::newStringUTF(const char *buf, bool global) {
    jstring result = env->NewStringUTF(buf);
    if (NULL == result) {
        // Exception must have been raised.
        checkAndClearException();
        throw OMSException("Fail to create a Java string: it's highly possible that system is running out of memory");
    }

    if (global) {
        return makeGlobal(result);
    }

    return result;
}