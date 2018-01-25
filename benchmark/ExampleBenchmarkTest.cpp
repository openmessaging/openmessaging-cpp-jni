#include <string>
#include <benchmark/benchmark.h>

static void BM_StringCreation(benchmark::State &state) {
    while (state.KeepRunning()) {
        std::string empty_string;
    }
}

BENCHMARK(BM_StringCreation);

BENCHMARK_MAIN();