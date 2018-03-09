#ifndef OMS_NONSTANDARDKEYS_H
#define OMS_NONSTANDARDKEYS_H

#include "Namespace.h"
#include <string>

BEGIN_NAMESPACE_2(io, openmessaging)

        static const std::string CONSUMER_GROUP = "rmq.consumer.group";
        static const std::string PRODUCER_GROUP = "rmq.producer.group";
        static const std::string MAX_REDELIVERY_TIMES = "rmq.max.redelivery.times";
        static const std::string MESSAGE_CONSUME_TIMEOUT = "rmq.message.consume.timeout";
        static const std::string MAX_CONSUME_THREAD_NUMS = "rmq.max.consume.thread.nums";
        static const std::string MIN_CONSUME_THREAD_NUMS = "rmq.min.consume.thread.nums";
        static const std::string MESSAGE_CONSUME_STATUS = "rmq.message.consume.status";
        static const std::string MESSAGE_DESTINATION = "rmq.message.destination";
        static const std::string PULL_MESSAGE_BATCH_NUMS = "rmq.pull.message.batch.nums";
        static const std::string PULL_MESSAGE_CACHE_CAPACITY = "rmq.pull.message.cache.capacity";

END_NAMESPACE_2(io, openmessaging)

#endif //OMS_NONSTANDARDKEYS_H
