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
                if (NULL == localDefaultKeyValueClass) {
                    BOOST_LOG_TRIVIAL(warning) << "io/openmessaging/internal/DefaultKeyValue is not found";
                    abort();
                }
                defaultKeyValueClass = reinterpret_cast<jclass>(current.env->NewGlobalRef(localDefaultKeyValueClass));

                defaultKeyValueCtor = current.env->GetMethodID(defaultKeyValueClass, "<init>", "()V");
                if (NULL == defaultKeyValueCtor) {
                    BOOST_LOG_TRIVIAL(warning) << "Default constructor of io/openmessaging/internal/DefaultKeyValue is not found";
                    abort();
                }

                jobject localDefaultKeyValueObject = current.env->NewObject(defaultKeyValueClass, defaultKeyValueCtor);
                if (NULL != localDefaultKeyValueObject) {
                    defaultKeyValueObject = current.env->NewGlobalRef(localDefaultKeyValueObject);
                } else {
                    BOOST_LOG_TRIVIAL(warning) << "Unable to create object for io/openmessaging/internal/DefaultKeyValue";
                    abort();
                }

                putInt = current.env->GetMethodID(defaultKeyValueClass, "put", "(Ljava/lang/String;I)Lio/openmessaging/KeyValue");
                if (NULL == putInt) {
                    BOOST_LOG_TRIVIAL(warning) << "put(String, int) is not found for io/openmessaging/internal/DefaultKeyValue";
                    abort();
                }

            }


            KeyValueImpl::~KeyValueImpl() {
                CurrentEnv current;
                if (defaultKeyValueClass) {
                    current.env->DeleteGlobalRef(defaultKeyValueClass);
                    defaultKeyValueClass = NULL;
                }
            }


            KeyValue& KeyValueImpl::put(const std::string &key, int value) {
                CurrentEnv current;
                jstring k = current.env->NewStringUTF(key.c_str());
                current.env->CallObjectMethod(defaultKeyValueObject, putInt, k, value);
                return *this;
            }

            KeyValue& KeyValueImpl::put(const std::string &key, long value) {
                return *this;
            }

            KeyValue& KeyValueImpl::put(const std::string &key, double value) {
                return *this;
            }

            KeyValue& KeyValueImpl::put(const std::string &key, const std::string &value) {
                return *this;
            }

            int KeyValueImpl::getInt(const std::string &key, int defaultValue) {
                return 0;
            }

            long KeyValueImpl::getLong(const std::string &key, long defaultValue) {
                return 0L;
            }

            double KeyValueImpl::getDouble(const std::string &key, double defaultValue) {
                return 0.0;
            }

            std::string KeyValueImpl::getString(const std::string &key, const std::string &defaultValue) {
                return std::string("");
            }

            std::set<std::string> KeyValueImpl::keySet() {
                std::set<std::string> s;
                return s;
            }

            bool KeyValueImpl::containsKey(const std::string &key) {
                return true;
            }
        }
    }
}