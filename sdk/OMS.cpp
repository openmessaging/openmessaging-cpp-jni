#include "OMS.h"
#include "core.h"
#include "KeyValueImpl.h"

using namespace io::openmessaging;

KeyValue* newKeyValueImpl() {
    Initialize();
    CurrentEnv current;
    const char *klassDefaultKeyValue = "io/openmessaging/internal/DefaultKeyValue";
    jclass classDefaultKeyValue = current.findClass(klassDefaultKeyValue);
    jmethodID ctor = current.getMethodId(classDefaultKeyValue, "<init>", "()V");
    jobject objectDefaultKeyValue = current.newObject(classDefaultKeyValue, ctor);
    return new KeyValueImpl(objectDefaultKeyValue);
}

boost::shared_ptr<KeyValue> OMS::newKeyValue() {
    KeyValue* kv = ::newKeyValueImpl();
    return boost::shared_ptr<KeyValue>(kv);
}