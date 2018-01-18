#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "KeyValue.h"
#include "BuiltinKeys.h"
#include "Namespace.h"

BEGIN_NAMESPACE_2(io, openmessaging)
        class Message {
        public:
            virtual ~Message() {

            }

            virtual boost::shared_ptr<KeyValue> sysHeaders() = 0;

            virtual boost::shared_ptr<KeyValue> userHeaders() = 0;

            virtual Message& putSysHeaders(const std::string &key, int value) = 0;

            virtual Message& putSysHeaders(const std::string &key, long value) = 0;

            virtual Message& putSysHeaders(const std::string &key, double value) = 0;

            virtual Message& putSysHeaders(const std::string &key, const std::string &value) = 0;

            virtual Message& putUserHeaders(const std::string &key, int value) = 0;

            virtual Message& putUserHeaders(const std::string &key, long value) = 0;

            virtual Message& putUserHeaders(const std::string &key, double value) = 0;

            virtual Message& putUserHeaders(const std::string &key, const std::string &value) = 0;

        };

END_NAMESPACE_2(io, openmessaging)

#endif // MESSAGE_H
