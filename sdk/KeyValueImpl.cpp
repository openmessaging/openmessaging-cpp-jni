#include "KeyValueImpl.h"

using namespace io::openmessaging;

KeyValueImpl::KeyValueImpl() {
    CurrentEnv current;
    const char *klassDefaultKeyValue = "io/openmessaging/internal/DefaultKeyValue";
    classDefaultKeyValue = current.findClass(klassDefaultKeyValue);
    init(current);
    defaultKeyValueCtor = current.getMethodId(classDefaultKeyValue, "<init>", "()V");
    defaultKeyValueObject = current.newObject(classDefaultKeyValue, defaultKeyValueCtor);
}

KeyValueImpl::KeyValueImpl(jobject proxy) : defaultKeyValueObject(proxy) {
    CurrentEnv current;
    classDefaultKeyValue = current.getObjectClass(proxy);
    init(current);
}

void KeyValueImpl::init(CurrentEnv &current) {
    putInt = current.getMethodId(classDefaultKeyValue, "put", "(Ljava/lang/String;I)Lio/openmessaging/KeyValue;");
    putLong = current.getMethodId(classDefaultKeyValue, "put", "(Ljava/lang/String;J)Lio/openmessaging/KeyValue;");
    putDouble = current.getMethodId(classDefaultKeyValue, "put", "(Ljava/lang/String;D)Lio/openmessaging/KeyValue;");
    putString = current.getMethodId(classDefaultKeyValue, "put", "(Ljava/lang/String;Ljava/lang/String;)Lio/openmessaging/KeyValue;");
    getIntMethod = current.getMethodId(classDefaultKeyValue, "getInt", "(Ljava/lang/String;)I");
    getLongMethod = current.getMethodId(classDefaultKeyValue, "getLong", "(Ljava/lang/String;)J");
    getDoubleMethod = current.getMethodId(classDefaultKeyValue, "getDouble", "(Ljava/lang/String;)D");
    getStringMethod = current.getMethodId(classDefaultKeyValue, "getString", "(Ljava/lang/String;)Ljava/lang/String;");
    midContainsKey = current.getMethodId(classDefaultKeyValue, "containsKey", "(Ljava/lang/String;)Z");

    keySetMethod = current.getMethodId(classDefaultKeyValue, "keySet", "()Ljava/util/Set;");
}

KeyValueImpl::~KeyValueImpl() {
    CurrentEnv current;
    if (classDefaultKeyValue) {
        current.deleteRef(classDefaultKeyValue);
        classDefaultKeyValue = NULL;
    }

    if (!current.env->IsSameObject(defaultKeyValueObject, NULL)) {
        current.deleteRef(defaultKeyValueObject);
    }
}

KeyValue &KeyValueImpl::put(const std::string &key, int value) {
    CurrentEnv current;
    jstring k = current.newStringUTF(key.c_str());
    jobject obj = current.callObjectMethod(defaultKeyValueObject, putInt, k, value);

    current.deleteRef(k);
    if (obj) {
        current.deleteRef(obj);
    }

    return *this;
}

KeyValue &KeyValueImpl::put(const std::string &key, long value) {
    CurrentEnv current;
    jstring k = current.newStringUTF(key.c_str());
    jobject obj = current.callObjectMethod(defaultKeyValueObject, putLong, k, value);
    current.deleteRef(k);
    if (obj) {
        current.deleteRef(obj);
    }
    return *this;
}

KeyValue &KeyValueImpl::put(const std::string &key, double value) {
    CurrentEnv current;
    jstring k = current.newStringUTF(key.c_str());
    jobject obj = current.callObjectMethod(defaultKeyValueObject, putDouble, k, value);
    current.deleteRef(k);
    if (obj) {
        current.deleteRef(obj);
    }
    return *this;
}

KeyValue &KeyValueImpl::put(const std::string &key, const std::string &value) {
    CurrentEnv current;

    jstring k = current.newStringUTF(key.c_str());
    jstring v = current.newStringUTF(value.c_str());

    jobject obj = current.callObjectMethod(defaultKeyValueObject, putString, k, v);

    if (obj) {
        current.deleteRef(obj);
    }

    current.deleteRef(k);
    current.deleteRef(v);
    return *this;
}

int KeyValueImpl::getInt(const std::string &key, int defaultValue) {
    CurrentEnv current;
    jstring k = current.newStringUTF(key.c_str());
    jint value = current.callIntMethod(defaultKeyValueObject, getIntMethod, k);
    current.deleteRef(k);
    return value;
}

long KeyValueImpl::getLong(const std::string &key, long defaultValue) {
    CurrentEnv current;
    jstring k = current.newStringUTF(key.c_str());
    long value = current.callLongMethod(defaultKeyValueObject, getLongMethod, k);
    current.deleteRef(k);
    return value;
}

double KeyValueImpl::getDouble(const std::string &key, double defaultValue) {
    CurrentEnv current;
    jstring k = current.newStringUTF(key.c_str());
    jdouble value = current.callDoubleMethod(defaultKeyValueObject, getDoubleMethod, k);
    current.deleteRef(k);
    return value;
}

std::string KeyValueImpl::getString(const std::string &key, const std::string &defaultValue) {
    CurrentEnv current;
    jstring k = current.newStringUTF(key.c_str());
    jstring value = reinterpret_cast<jstring>(current.callObjectMethod(defaultKeyValueObject, getStringMethod, k));
    current.deleteRef(k);
    const char *data = current.env->GetStringUTFChars(value, NULL);
    if (NULL == data) {
        return defaultValue;
    }

    std::string ret = data;
    current.env->ReleaseStringUTFChars(value, data);
    current.deleteRef(value);
    return ret;
}

std::set<std::string> KeyValueImpl::keySet() {
    CurrentEnv current;
    jobject jKeySet = current.callObjectMethod(defaultKeyValueObject, keySetMethod);
    std::set<std::string> result = toNativeSet(current, jKeySet);
    current.deleteRef(jKeySet);
    return result;
}

bool KeyValueImpl::containsKey(const std::string &key) {
    CurrentEnv current;
    jstring k = current.newStringUTF(key.c_str());
    bool contains = current.callBooleanMethod(defaultKeyValueObject, midContainsKey, k);
    current.deleteRef(k);
    return contains;
}

jobject KeyValueImpl::getProxy() {
    return defaultKeyValueObject;
}

