#ifndef OMS_BUILTIN_KEYS_H
#define OMS_BUILTIN_KEYS_H

#include <string>
#include "Namespace.h"

BEGIN_NAMESPACE_2(io, openmessaging)

        static const std::string MESSAGE_ID = "MESSAGE_ID";
        static const std::string DESTINATION = "DESTINATION";
        static const std::string RECEIPT_HANDLE = "RECEIPT_HANDLE";
        static const std::string BORN_TIMESTAMP = "BORN_TIMESTAMP";
        static const std::string BORN_HOST = "BORN_HOST";
        static const std::string STORE_TIMESTAMP = "STORE_TIMESTAMP";
        static const std::string STORE_HOST = "STORE_HOST";
        static const std::string START_TIME = "START_TIME";
        static const std::string STOP_TIME = "STOP_TIME";
        static const std::string TIMEOUT = "TIMEOUT";
        static const std::string PRIORITY = "PRIORITY";
        static const std::string RELIABILITY = "RELIABILITY";
        static const std::string SEARCH_KEYS = "SEARCH_KEYS";
        static const std::string SCHEDULE_EXPRESSION = "SCHEDULE_EXPRESSION";
        static const std::string TRACE_ID = "TRACE_ID";
        static const std::string STREAM_KEY = "STREAM_KEY";
        static const std::string REDELIVERED_NUMBER = "REDELIVERED_NUMBER";
        static const std::string REDELIVERED_REASON = "REDELIVERED_REASON";

END_NAMESPACE_2(io, openmessaging)
#endif // OMS_BUILTIN_KEYS_H
