#ifndef OMS_KEYVALUEIMPL_H
#define OMS_KEYVALUEIMPL_H

#include "core.h"
#include "KeyValue.h"

namespace io {
    namespace openmessaging {
        namespace core {
            class KeyValueImpl : public virtual KeyValue {
            public:
                KeyValueImpl();

                virtual ~KeyValueImpl();

                virtual KeyValue& put(const std::string &key, int value);

                virtual KeyValue& put(const std::string &key, long value);

                virtual KeyValue& put(const std::string &key, double value);

                virtual KeyValue& put(const std::string &key, const std::string &value);

                virtual int getInt(const std::string &key, int defaultValue);

                virtual long getLong(const std::string &key, long defaultValue);

                virtual double getDouble(const std::string &key, double defaultValue);

                virtual std::string getString(const std::string &key, const std::string &defaultValue);

                virtual std::set<std::string> keySet();

                virtual bool containsKey(const std::string &key);

            private:
                jclass defaultKeyValueClass;
                jmethodID defaultKeyValueCtor;
                jmethodID putInt;

                jobject defaultKeyValueObject;

            };
        }
    }
}

#endif //OMS_KEYVALUEIMPL_H
