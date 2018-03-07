#ifndef OMS_NONSTANDARDKEYS_H
#define OMS_NONSTANDARDKEYS_H

#include "Namespace.h"
#include <string>

BEGIN_NAMESPACE_2(io, openmessaging)

    class NonStandardKeys {
    public:
        static const std::string CONSUMER_GROUP;
        static const std::string PRODUCER_GROUP;
        static const std::string MAX_REDELIVERY_TIMES;
        static const std::string MESSAGE_CONSUME_TIMEOUT;
        static const std::string MAX_CONSUME_THREAD_NUMS;
        static const std::string MIN_CONSUME_THREAD_NUMS;
        static const std::string MESSAGE_CONSUME_STATUS;
        static const std::string MESSAGE_DESTINATION;
        static const std::string PULL_MESSAGE_BATCH_NUMS;
        static const std::string PULL_MESSAGE_CACHE_CAPACITY;
    };

END_NAMESPACE_2(io, openmessaging)

#endif //OMS_NONSTANDARDKEYS_H
