#include "OMS.h"
#include "core.h"
#include "KeyValueImpl.h"

using namespace io::openmessaging;

BEGIN_NAMESPACE_2(io, openmessaging)
    boost::shared_ptr<KeyValue> kv_nullptr;
END_NAMESPACE_2(io, openmessaging)

boost::shared_ptr<KeyValue> OMS::newKeyValue() {
    CurrentEnv current;
    jclass classDefaultKeyValue = current.env->FindClass("io/openmessaging/internal/DefaultKeyValue");
    jmethodID ctor = getMethod(current, classDefaultKeyValue, "<init>", "()V");
    jobject objectDefaultKeyValue = current.env->NewObject(classDefaultKeyValue, ctor);
    current.checkAndClearException();
    boost::shared_ptr<KeyValue> ptr = boost::make_shared<KeyValueImpl>(
            current.env->NewGlobalRef(objectDefaultKeyValue));
    current.env->DeleteLocalRef(objectDefaultKeyValue);
    return ptr;
}