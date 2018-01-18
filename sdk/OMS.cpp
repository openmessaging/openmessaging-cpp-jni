#include "Namespace.h"
#include "OMS.h"
#include "core.h"
#include "KeyValueImpl.h"

BEGIN_NAMESPACE_2(io, openmessaging)

    boost::shared_ptr<KeyValue> OMS::newKeyValue() {
        CurrentEnv current;
        jclass classDefaultKeyValue = current.env->FindClass("io/openmessaging/internal/DefaultKeyValue");
        jmethodID ctor = getMethod(current, classDefaultKeyValue, "<init>", "()V");
        jobject objectDefaultKeyValue = current.env->NewObject(classDefaultKeyValue, ctor);
        boost::shared_ptr<KeyValue> ptr = boost::make_shared<KeyValueImpl>(current.env->NewGlobalRef(objectDefaultKeyValue));
        current.env->DeleteLocalRef(objectDefaultKeyValue);
        return ptr;
    }

END_NAMESPACE_2(io, openmessaging)