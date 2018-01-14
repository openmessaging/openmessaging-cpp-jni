#ifndef KEYVALUE_H
#define KEYVALUE_H

#include <string>
#include <map>
#include <set>

namespace io {
    namespace openmessaging {
        class KeyValue {
        public:
            virtual ~KeyValue() {

            }

            virtual KeyValue& put(const std::string &key, int value) = 0;

            virtual KeyValue& put(const std::string &key, long value) = 0;

            virtual KeyValue& put(const std::string &key, double value) = 0;

            virtual KeyValue& put(const std::string &key, const std::string &value) = 0;

            virtual int getInt(const std::string &key, int defaultValue = 0) = 0;

            virtual long getLong(const std::string &key, long defaultValue = 0L) = 0;

            virtual double getDouble(const std::string &key, double defaultValue = 0.0) = 0;

            virtual std::string getString(const std::string &key, const std::string &defaultValue = "") = 0;

            virtual std::set<std::string> keySet() = 0;

            virtual bool containsKey(const std::string &key) = 0;

        };
    }

}

#endif // KEYVALUE_H
