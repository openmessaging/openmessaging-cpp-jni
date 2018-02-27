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
    jmethodID ctor = current.getMethodId(classDefaultKeyValue, "<init>", "()V");
    jobject objectDefaultKeyValue = current.newObject(classDefaultKeyValue, ctor);
    boost::shared_ptr<KeyValue> ptr = boost::make_shared<KeyValueImpl>(objectDefaultKeyValue);
    return ptr;
}