#include <benchmark/benchmark.h>
#include <iostream>
#include <string>
#include <cassert>

#include "fcontext.hpp"

int value = 0;

void boostFcontextTask(transfer_t t){
    transfer_t tt {t.fctx, t.data};
    while(1){
        benchmark::DoNotOptimize(++value);
        tt = jump_fcontext(tt.fctx, tt.data);
    }
}

void boostFcontext(benchmark::State& state){
    char stack[4096];
    fcontext_t ctx = make_fcontext(&stack[4095], 4096, boostFcontextTask);

    transfer_t tt {ctx, nullptr};
    for(auto _ : state){
        tt = jump_fcontext(tt.fctx, tt.data);
        }
}

BENCHMARK(boostFcontext) -> Unit(benchmark::TimeUnit::kNanosecond);

BENCHMARK_MAIN();