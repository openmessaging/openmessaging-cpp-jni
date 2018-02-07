#include "KeyValueImpl.h"

using namespace io::openmessaging;

KeyValueImpl::KeyValueImpl() {
    CurrentEnv current;
    jclass classDefaultKeyValueLocal = current.env->FindClass("io/openmessaging/internal/DefaultKeyValue");
    if (!classDefaultKeyValueLocal) {
        BOOST_LOG_TRIVIAL(warning) << "Class io/openmessaging/internal/DefaultKeyValue is not found";
        abort();
    }

    classDefaultKeyValue = reinterpret_cast<jclass>(current.env->NewGlobalRef(classDefaultKeyValueLocal));
    current.env->DeleteLocalRef(classDefaultKeyValueLocal);

    init(current);

    defaultKeyValueCtor = getMethod(current, classDefaultKeyValue, "<init>", "()V");
    jobject localDefaultKeyValueObject = current.env->NewObject(classDefaultKeyValue, defaultKeyValueCtor);

    if (current.env->ExceptionCheck()) {
        current.env->ExceptionDescribe();
        current.env->ExceptionClear();
        abort();
    }

    if (localDefaultKeyValueObject) {
        defaultKeyValueObject = current.env->NewGlobalRef(localDefaultKeyValueObject);
        current.env->DeleteLocalRef(localDefaultKeyValueObject);
    } else {
        BOOST_LOG_TRIVIAL(warning) << "Unable to create object for io/openmessaging/internal/DefaultKeyValue";
        abort();
    }
}

KeyValueImpl::KeyValueImpl(jobject proxy) : defaultKeyValueObject(proxy) {
    CurrentEnv current;

    jclass classDefaultKeyValueLocal = current.env->GetObjectClass(proxy);
    current.checkAndClearException();
    classDefaultKeyValue = reinterpret_cast<jclass>(current.env->NewGlobalRef(classDefaultKeyValueLocal));
    current.env->DeleteLocalRef(classDefaultKeyValueLocal);
    current.checkAndClearException();

    init(current);
}

void KeyValueImpl::init(CurrentEnv &current) {
    putInt = getMethod(current, classDefaultKeyValue, "put", "(Ljava/lang/String;I)Lio/openmessaging/KeyValue;");
    putLong = getMethod(current, classDefaultKeyValue, "put", "(Ljava/lang/String;J)Lio/openmessaging/KeyValue;");
    putDouble = getMethod(current, classDefaultKeyValue, "put", "(Ljava/lang/String;D)Lio/openmessaging/KeyValue;");
    putString = getMethod(current, classDefaultKeyValue, "put",
                          "(Ljava/lang/String;Ljava/lang/String;)Lio/openmessaging/KeyValue;");

    getIntMethod = getMethod(current, classDefaultKeyValue, "getInt", "(Ljava/lang/String;)I");
    getLongMethod = getMethod(current, classDefaultKeyValue, "getLong", "(Ljava/lang/String;)J");
    getDoubleMethod = getMethod(current, classDefaultKeyValue, "getDouble", "(Ljava/lang/String;)D");
    getStringMethod = getMethod(current, classDefaultKeyValue, "getString", "(Ljava/lang/String;)Ljava/lang/String;");
    midContainsKey = getMethod(current, classDefaultKeyValue, "containsKey", "(Ljava/lang/String;)Z");

    keySetMethod = getMethod(current, classDefaultKeyValue, "keySet", "()Ljava/util/Set;");
}

KeyValueImpl::~KeyValueImpl() {
    CurrentEnv current;
    if (classDefaultKeyValue) {
        current.env->DeleteGlobalRef(classDefaultKeyValue);
        classDefaultKeyValue = NULL;
    }

    if (!current.env->IsSameObject(defaultKeyValueObject, NULL)) {
        current.env->DeleteGlobalRef(defaultKeyValueObject);
    }
}

KeyValue &KeyValueImpl::put(const std::string &key, int value) {
    CurrentEnv current;
    jstring k = current.env->NewStringUTF(key.c_str());
    jobject obj = current.env->CallObjectMethod(defaultKeyValueObject, putInt, k, value);
    current.env->DeleteLocalRef(k);

    if (obj) {
        current.env->DeleteLocalRef(obj);
    }
    return *this;
}

KeyValue &KeyValueImpl::put(const std::string &key, long value) {
    CurrentEnv current;
    jstring k = current.env->NewStringUTF(key.c_str());
    jobject obj = current.env->CallObjectMethod(defaultKeyValueObject, putLong, k, value);
    current.env->DeleteLocalRef(k);
    if (obj) {
        current.env->DeleteLocalRef(obj);
    }
    return *this;
}

KeyValue &KeyValueImpl::put(const std::string &key, double value) {
    CurrentEnv current;
    jstring k = current.env->NewStringUTF(key.c_str());
    jobject obj = current.env->CallObjectMethod(defaultKeyValueObject, putDouble, k, value);
    current.env->DeleteLocalRef(k);
    if (obj) {
        current.env->DeleteLocalRef(obj);
    }
    return *this;
}

KeyValue &KeyValueImpl::put(const std::string &key, const std::string &value) {
    CurrentEnv current;
    jstring k = current.env->NewStringUTF(key.c_str());
    jstring v = current.env->NewStringUTF(value.c_str());
    jobject obj = current.env->CallObjectMethod(defaultKeyValueObject, putString, k, v);

    if (obj) {
        current.env->DeleteLocalRef(obj);
    }

    if (current.env->ExceptionCheck()) {
        current.env->ExceptionDescribe();
        current.env->ExceptionClear();
    }

    current.env->DeleteLocalRef(k);
    current.env->DeleteLocalRef(v);
    return *this;
}

int KeyValueImpl::getInt(const std::string &key, int defaultValue) {
    CurrentEnv current;
    jstring k = current.env->NewStringUTF(key.c_str());
    jint value = current.env->CallIntMethod(defaultKeyValueObject, getIntMethod, k);
    current.env->DeleteLocalRef(k);
    if (current.env->ExceptionCheck()) {
        current.env->ExceptionDescribe();
        current.env->ExceptionClear();
        return defaultValue;
    }
    return value;
}

long KeyValueImpl::getLong(const std::string &key, long defaultValue) {
    CurrentEnv current;
    jstring k = current.env->NewStringUTF(key.c_str());
    jlong value = current.env->CallLongMethod(defaultKeyValueObject, getLongMethod, k);
    current.env->DeleteLocalRef(k);
    if (current.env->ExceptionCheck()) {
        current.env->ExceptionDescribe();
        current.env->ExceptionClear();
        return defaultValue;
    }
    return value;
}

double KeyValueImpl::getDouble(const std::string &key, double defaultValue) {
    CurrentEnv current;
    jstring k = current.env->NewStringUTF(key.c_str());
    jdouble value = current.env->CallDoubleMethod(defaultKeyValueObject, getDoubleMethod, k);
    current.env->DeleteLocalRef(k);
    if (current.env->ExceptionCheck()) {
        current.env->ExceptionDescribe();
        current.env->ExceptionClear();
        return defaultValue;
    }
    return value;
}

std::string KeyValueImpl::getString(const std::string &key, const std::string &defaultValue) {
    CurrentEnv current;
    jstring k = current.env->NewStringUTF(key.c_str());
    jstring value = reinterpret_cast<jstring>(current.env->CallObjectMethod(defaultKeyValueObject, getStringMethod, k));
    current.env->DeleteLocalRef(k);
    if (current.env->ExceptionCheck()) {
        current.env->ExceptionDescribe();
        current.env->ExceptionClear();
        return defaultValue;
    }

    const char *data = current.env->GetStringUTFChars(value, NULL);
    if (NULL == data) {
        return defaultValue;
    }

    std::string ret = data;
    current.env->ReleaseStringUTFChars(value, data);
    current.env->DeleteLocalRef(value);
    return ret;
}

std::set<std::string> KeyValueImpl::keySet() {
    CurrentEnv current;
    jobject jKeySet = current.env->CallObjectMethod(defaultKeyValueObject, keySetMethod);
    std::set<std::string> result = toNativeSet(current, jKeySet);
    current.env->DeleteLocalRef(jKeySet);
    return result;
}

bool KeyValueImpl::containsKey(const std::string &key) {
    CurrentEnv current;
    jstring k = current.env->NewStringUTF(key.c_str());
    jboolean contains = current.env->CallBooleanMethod(defaultKeyValueObject, midContainsKey, k);
    current.env->DeleteLocalRef(k);
    return contains;
}

jobject KeyValueImpl::getProxy() {
    return defaultKeyValueObject;
}

