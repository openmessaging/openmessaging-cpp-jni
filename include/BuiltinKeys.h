#ifndef OMS_BUILTIN_KEYS_H
#define OMS_BUILTIN_KEYS_H

#include <string>
#include "Namespace.h"

BEGIN_NAMESPACE_2(io, openmessaging)

        static const std::string MessageId = "MessageId";
        static const std::string Topic = "Topic";
        static const std::string Queue = "Queue";
        static const std::string BornTimestamp = "BornTimestamp";
        static const std::string BornHost = "BornHost";
        static const std::string StoreTimestamp = "StoreTimestamp";
        static const std::string StoreHost = "StoreHost";
        static const std::string StartTime = "StartTime";
        static const std::string StopTime = "StopTime";
        static const std::string Timeout = "Timeout";
        static const std::string Priority = "Priority";
        static const std::string Reliability = "Reliability";
        static const std::string SearchKey = "SearchKey";
        static const std::string ScheduleExpression = "ScheduleExpression";
        static const std::string TraceId = "TraceId";
        static const std::string Stream = "Stream";

END_NAMESPACE_2(io, openmessaging)
#endif // OMS_BUILTIN_KEYS_H
