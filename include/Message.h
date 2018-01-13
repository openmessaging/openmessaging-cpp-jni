#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "KeyValue.h"
#include "BuiltinKeys.h"

namespace io {
    namespace openmessaging {
        class Message {
        public:
            Message() {
                _headers = boost::make_shared<KeyValue>();
                _properties = boost::make_shared<KeyValue>();
            }

            virtual ~Message() {

            }

            boost::shared_ptr<KeyValue> headers() {
                return _headers;
            }

            boost::shared_ptr<KeyValue> properties() {
                return _properties;
            }

            template<typename T>
            Message &putHeader(std::string &key, T value) {
                if (_headers) {
                    _headers->put<T>(key, value);
                }
                return *this;
            }

            template<typename T>
            Message &putProperty(std::string &key, T value) {
                if (_properties) {
                    _properties->put<T>(key, value);
                }
                return *this;
            }

        protected:
            boost::shared_ptr<KeyValue> _headers;

            boost::shared_ptr<KeyValue> _properties;
        };
    }

}

#endif // MESSAGE_H
