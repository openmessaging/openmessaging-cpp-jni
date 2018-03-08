#include "OMS.h"
#include "core.h"
#include "KeyValueImpl.h"

using namespace io::openmessaging;

BEGIN_NAMESPACE_2(io, openmessaging)
    boost::shared_ptr<KeyValue> kv_nullptr;
END_NAMESPACE_2(io, openmessaging)

KeyValue* newKeyValue() {
    CurrentEnv current;
    const char *klassDefaultKeyValue = "io/openmessaging/internal/DefaultKeyValue";
    jclass classDefaultKeyValue = current.findClass(klassDefaultKeyValue);
    jmethodID ctor = current.getMethodId(classDefaultKeyValue, "<init>", "()V");
    jobject objectDefaultKeyValue = current.newObject(classDefaultKeyValue, ctor);
    return new KeyValueImpl(objectDefaultKeyValue);
}

boost::shared_ptr<KeyValue> OMS::newKeyValue() {
    KeyValue* kv = ::newKeyValue();
    return boost::shared_ptr<KeyValue>(kv);
}