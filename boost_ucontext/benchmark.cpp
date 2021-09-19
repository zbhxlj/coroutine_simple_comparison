#include <benchmark/benchmark.h>
#include <iostream>
#include <string>
#include <cassert>

#include <ucontext.h>

int value = 0;

ucontext_t taskCtx;
ucontext_t mainCtx;

void boostUcontextTask(){
    while(1){
        benchmark::DoNotOptimize(++value);
        swapcontext(&taskCtx, &mainCtx);
    }
}

void boostUcontext(benchmark::State& state){
    char stack[4096];

    assert(getcontext(&taskCtx) == 0);

    taskCtx.uc_link = nullptr;
    taskCtx.uc_stack.ss_sp = &stack[4095];
    taskCtx.uc_stack.ss_size = 4096;

    makecontext(&taskCtx, &boostUcontextTask, 0);

    for(auto _ : state){
        swapcontext(&mainCtx, &taskCtx);
    }
}

BENCHMARK(boostUcontext) -> Unit(benchmark::TimeUnit::kNanosecond);

BENCHMARK_MAIN();