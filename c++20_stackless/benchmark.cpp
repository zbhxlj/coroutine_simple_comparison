#include <benchmark/benchmark.h>
#include "generator.h"

int value = 0;
generator<int> cpp20Task(){
    while(true){
        benchmark::DoNotOptimize(++value);
        co_yield value;
    }
}
void cpp20(benchmark::State& state){
    auto g = cpp20Task();
    auto it = g.begin();
    for(auto _ : state){
        benchmark::DoNotOptimize(++it);
    }
}

BENCHMARK(cpp20) -> Unit(benchmark::TimeUnit::kNanosecond);

BENCHMARK_MAIN();