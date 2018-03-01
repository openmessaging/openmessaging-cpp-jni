#ifndef OMS_CURRENT_ENV_H
#define OMS_CURRENT_ENV_H

#include "Namespace.h"
#include "Uncopyable.h"
#include "OMSException.h"

BEGIN_NAMESPACE_2(io, openmessaging)
    class CurrentEnv : private Uncopyable {
    public:

        CurrentEnv();

        CurrentEnv(JNIEnv *ctx);

        ~CurrentEnv();

        JNIEnv *env;

        template<typename T>
        T makeGlobal(T localRef) {
            T globalRef = static_cast<T>(env->NewGlobalRef(localRef));
            env->DeleteLocalRef(localRef);
            return boost::move(globalRef);
        }

        bool checkAndClearException();

        jobject callObjectMethod(jobject obj, jmethodID mid, ...);

        jobject callStaticObjectMethod(jclass clazz, jmethodID mid, ...);

        void callVoidMethod(jobject obj, jmethodID mid, ...);

        bool callBooleanMethod(jobject obj, jmethodID mid, ...);

        int callIntMethod(jobject obj, jmethodID mid, ...);

        long callLongMethod(jobject obj, jmethodID mid, ...);

        double callDoubleMethod(jobject obj, jmethodID mid, ...);

        template <typename T>
        T newGlobalRef(T obj) {
            if (NULL == obj) {
                throw OMSException("Pass-in object is null");
            }

            T globalRef = NULL;
            if (env->GetObjectRefType(obj) == JNILocalRefType) {
                globalRef = reinterpret_cast<T>(env->NewGlobalRef(obj));
                if (NULL == globalRef) {
                    throw OMSException("Failed to create global reference");
                }
                env->DeleteLocalRef(obj);
                // check JNI exception?
                return globalRef;
            }

            return obj;
        }

        jclass findClass(const char *clazz);

        jclass getObjectClass(jobject obj);

        jmethodID getMethodId(jclass klass, const char *method, const char* sig, bool is_static = false);

        jobject newObject(jclass klass, jmethodID mid, ...);

        template <typename T>
        void deleteRef(T t) {

            _jobjectType type = env->GetObjectRefType(t);

            if (type == JNIGlobalRefType) {
                env->DeleteGlobalRef(t);
                return;
            }

            if (type == JNILocalRefType) {
                env->DeleteLocalRef(t);
                return;
            }

            BOOST_LOG_TRIVIAL(warning) << "Unexpected reference type to delete";
        }

        jstring newStringUTF(const char *buf, bool global = false);

    private:
        bool attached;
    };

END_NAMESPACE_2(io, openmessaging)


#endif //OMS_CURRENT_ENV_H
