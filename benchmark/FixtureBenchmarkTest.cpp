#include <string>
#include <benchmark/benchmark.h>
#include <boost/thread.hpp>

class ExampleFixture : public benchmark::Fixture {
public:
    ExampleFixture() : _mtx() {

    }

    void countUp() {
        boost::lock_guard<boost::mutex> lk(_mtx);
        count++;
    }

private:
    unsigned long long count;
    boost::mutex _mtx;
};

BENCHMARK_F(ExampleFixture, FooTest)(benchmark::State &state) {
    while (state.KeepRunning()) {
        countUp();
    }
}

BENCHMARK_REGISTER_F(ExampleFixture, FooTest)->Threads(2);

BENCHMARK_MAIN();