#ifndef OMS_KEY_VALUE_IMPL_H
#define OMS_KEY_VALUE_IMPL_H

#include "core.h"
#include "KeyValue.h"
#include "Namespace.h"

BEGIN_NAMESPACE_2(io, openmessaging)
    class KeyValueImpl : public virtual KeyValue {
    public:
        KeyValueImpl();

        KeyValueImpl(jobject proxy);

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

        jobject getProxy();

    private:
        jclass classDefaultKeyValue;
        jmethodID defaultKeyValueCtor;
        jmethodID putInt;
        jmethodID putLong;
        jmethodID putDouble;
        jmethodID putString;

        jmethodID getIntMethod;
        jmethodID getLongMethod;
        jmethodID getDoubleMethod;
        jmethodID getStringMethod;
        jmethodID keySetMethod;
        jmethodID midContainsKey;

        jobject defaultKeyValueObject;

        void init(CurrentEnv &current);
    };
    typedef NS::shared_ptr<KeyValueImpl> KeyValueImplPtr;

END_NAMESPACE_2(io, openmessaging)

#endif //OMS_KEY_VALUE_IMPL_H
