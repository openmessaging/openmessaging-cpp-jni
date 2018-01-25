#include <string>
#include <benchmark/benchmark.h>
#include <boost/smart_ptr.hpp>
#include "producer/Producer.h"
#include "MessagingAccessPointFactory.h"
#include "KeyValueImpl.h"

using namespace io::openmessaging;

class ProducerImplBenchmarkTest : public benchmark::Fixture {
public:
    ProducerImplBenchmarkTest() {
        std::string accessPointUrl = "oms:rocketmq://localhost:9876/default:default";
        std::string driverClassKey = "oms.driver.impl";
        std::string driverClass = "io.openmessaging.rocketmq.MessagingAccessPointImpl";

        Initialize();

        boost::shared_ptr<KeyValue> properties = boost::make_shared<KeyValueImpl>();
        properties->put(driverClassKey, driverClass);

        boost::shared_ptr<MessagingAccessPoint> messagingAccessPoint =
                MessagingAccessPointFactory::getMessagingAccessPoint(accessPointUrl, properties);

        producer = messagingAccessPoint->createProducer();
        producer->startup();
    }

    boost::shared_ptr<producer::Producer> producer;
};

BENCHMARK_DEFINE_F(ProducerImplBenchmarkTest, SynchronousSendTest)(benchmark::State &state) {
    std::string topic = "TopicTest";
    unsigned char* data = new unsigned char[state.range(0)];
    memset(data, 'x', state.range(0));
    std::vector<unsigned char> body(data, data + state.range(0));
    delete[] data;

    boost::shared_ptr<Message> message = producer->createByteMessageToTopic(topic, body);

    while (state.KeepRunning()) {
        boost::shared_ptr<producer::SendResult> sendResult = producer->send(message);
    }
}

BENCHMARK_REGISTER_F(ProducerImplBenchmarkTest, SynchronousSendTest)
->RangeMultiplier(2)
->Range(1<<8, 1<<16)
->Threads(2)
->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();