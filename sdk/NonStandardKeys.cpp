#include "NonStandardKeys.h"

using namespace io::openmessaging;
using namespace std;

const string NonStandardKeys::CONSUMER_GROUP = "rmq.consumer.group";
const string NonStandardKeys::PRODUCER_GROUP = "rmq.producer.group";
const string NonStandardKeys::MAX_REDELIVERY_TIMES = "rmq.max.redelivery.times";
const string NonStandardKeys::MESSAGE_CONSUME_TIMEOUT = "rmq.message.consume.timeout";
const string NonStandardKeys::MAX_CONSUME_THREAD_NUMS = "rmq.max.consume.thread.nums";
const string NonStandardKeys::MIN_CONSUME_THREAD_NUMS = "rmq.min.consume.thread.nums";
const string NonStandardKeys::MESSAGE_CONSUME_STATUS = "rmq.message.consume.status";
const string NonStandardKeys::MESSAGE_DESTINATION = "rmq.message.destination";
const string NonStandardKeys::PULL_MESSAGE_BATCH_NUMS = "rmq.pull.message.batch.nums";
const string NonStandardKeys::PULL_MESSAGE_CACHE_CAPACITY = "rmq.pull.message.cache.capacity";
