#ifndef KEYVALUE_H
#define KEYVALUE_H

#include <string>
#include <map>
#include <set>

#include <boost/lexical_cast.hpp>

namespace io {
    namespace openmessaging {

        using namespace std;

        class KeyValue {
        public:

            template<typename T>
            KeyValue &put(const string &key, T value) {
                m[key] = to_string(value);
                return *this;
            }

            template<typename T>
            T get(const string &key) {
                map<string, string>::iterator it = m.find(key);
                if (it != m.end()) {
                    return boost::lexical_cast<T>(m[key]);
                }

                return T();
            }

            set<string> keySet() {
                set<string> s;
                if (m.empty()) {
                    return s;
                }

                for (map<string, string>::const_iterator it = m.cbegin(); it != m.cend(); it++) {
                    s.insert(it->first);
                }

                return s;
            }

            bool containsKey(string key) {
                return m.find(key) != m.end();
            }

        protected:
            map<string, string> m;

        };

    }

}

#endif // KEYVALUE_H
