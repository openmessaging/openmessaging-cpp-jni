#include "KeyValueImpl.h"

namespace io {
    namespace openmessaging {
        namespace core {

            KeyValueImpl::KeyValueImpl() {
                if (!isRunning()) {
                    Initialize();
                }

                CurrentEnv current;
                jclass localDefaultKeyValueClass = current.env->FindClass("io/openmessaging/internal/DefaultKeyValue");
                if (!localDefaultKeyValueClass) {
                    BOOST_LOG_TRIVIAL(warning) << "Class io/openmessaging/internal/DefaultKeyValue is not found";
                    abort();
                }
                defaultKeyValueClass = reinterpret_cast<jclass>(current.env->NewGlobalRef(localDefaultKeyValueClass));

                defaultKeyValueCtor = getMethod(current, "<init>", "()V");
                jobject localDefaultKeyValueObject = current.env->NewObject(defaultKeyValueClass, defaultKeyValueCtor);
                if (localDefaultKeyValueObject) {
                    defaultKeyValueObject = current.env->NewGlobalRef(localDefaultKeyValueObject);
                } else {
                    BOOST_LOG_TRIVIAL(warning) << "Unable to create object for io/openmessaging/internal/DefaultKeyValue";
                    abort();
                }

                putInt = getMethod(current, "put", "(Ljava/lang/String;I)Lio/openmessaging/KeyValue;");
                putLong = getMethod(current, "put", "(Ljava/lang/String;J)Lio/openmessaging/KeyValue;");
                putDouble = getMethod(current, "put", "(Ljava/lang/String;D)Lio/openmessaging/KeyValue;");
                putString = getMethod(current, "put", "(Ljava/lang/String;Ljava/lang/String;)Lio/openmessaging/KeyValue;");

                getIntMethod = getMethod(current, "getInt", "(Ljava/lang/String;)I");
                getLongMethod = getMethod(current, "getLong", "(Ljava/lang/String;)J");
                getDoubleMethod = getMethod(current, "getDouble", "(Ljava/lang/String;)D");
                getStringMethod = getMethod(current, "getString", "(Ljava/lang/String;)Ljava/lang/String;");
                midContainsKey = getMethod(current, "containsKey", "(Ljava/lang/String;)Z");
            }




            KeyValueImpl::~KeyValueImpl() {
                CurrentEnv current;
                if (defaultKeyValueClass) {
                    current.env->DeleteGlobalRef(defaultKeyValueClass);
                    defaultKeyValueClass = NULL;
                }

                if (defaultKeyValueObject) {
                    current.env->DeleteGlobalRef(defaultKeyValueObject);
                }
            }


            KeyValue& KeyValueImpl::put(const std::string &key, int value) {
                CurrentEnv current;
                jstring k = current.env->NewStringUTF(key.c_str());
                current.env->CallObjectMethod(defaultKeyValueObject, putInt, k, value);
                current.env->DeleteLocalRef(k);
                return *this;
            }

            KeyValue& KeyValueImpl::put(const std::string &key, long value) {
                CurrentEnv current;
                jstring k = current.env->NewStringUTF(key.c_str());
                current.env->CallObjectMethod(defaultKeyValueObject, putLong, k, value);
                current.env->DeleteLocalRef(k);
                return *this;
            }

            KeyValue& KeyValueImpl::put(const std::string &key, double value) {
                CurrentEnv current;
                jstring k = current.env->NewStringUTF(key.c_str());
                current.env->CallObjectMethod(defaultKeyValueObject, putDouble, k, value);
                current.env->DeleteLocalRef(k);
                return *this;
            }

            KeyValue& KeyValueImpl::put(const std::string &key, const std::string &value) {
                CurrentEnv current;
                jstring k = current.env->NewStringUTF(key.c_str());
                jstring v = current.env->NewStringUTF(value.c_str());
                current.env->CallObjectMethod(defaultKeyValueObject, putString, k, v);
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

                const char* data = current.env->GetStringUTFChars(value, NULL);
                std::string ret = data;
                current.env->ReleaseStringUTFChars(value, data);
                return ret;
            }

            std::set<std::string> KeyValueImpl::keySet() {
                CurrentEnv current;
                keySetMethod = getMethod(current, "keySet", "()Ljava/util/Set;");
                jobject jKeySet = current.env->CallObjectMethod(defaultKeyValueObject, keySetMethod);
                return toNativeSet(current, jKeySet);
            }

            bool KeyValueImpl::containsKey(const std::string &key) {
                CurrentEnv current;
                jstring k = current.env->NewStringUTF(key.c_str());
                jboolean contains = current.env->CallBooleanMethod(defaultKeyValueObject, midContainsKey, k);
                current.env->DeleteLocalRef(k);
                return contains;
            }

            inline jmethodID KeyValueImpl::getMethod(CurrentEnv &current, const std::string &name,
                                                     const std::string &signature, bool isStatic) {
                jmethodID  methodId;
                if (isStatic) {
                    methodId = current.env->GetStaticMethodID(defaultKeyValueClass, name.c_str(), signature.c_str());
                } else {
                    methodId = current.env->GetMethodID(defaultKeyValueClass, name.c_str(), signature.c_str());
                }

                if (!methodId) {
                    BOOST_LOG_TRIVIAL(warning) << "Failed to GetMethodID. Method: " << name << ", Signature: " << signature;
                    abort();
                }

                return methodId;
            }
        }


    }
}
