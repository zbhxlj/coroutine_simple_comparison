#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include "coctx.h"
#include "co_routine.h"
#include "co_routine_inner.h"
#include "benchmark/benchmark.h"

int value = 0;

void* libcoTask(void* args){
	while (true)
	{
        benchmark::DoNotOptimize(++value);
        co_yield_ct();
	}
	return NULL;
}

void libco(benchmark::State& state){
    co_enable_hook_sys();
    stShareStack_t* share_stack= co_alloc_sharestack(1, 1024 * 128);
	stCoRoutineAttr_t attr;
	attr.stack_size = 0;
	attr.share_stack = share_stack;

    stCoRoutine_t* co;
	co_create(&co, &attr, libcoTask, nullptr);
	
    for(auto _ : state){
        co_resume(co);
    }
}

BENCHMARK(libco) -> Unit(benchmark::TimeUnit::kNanosecond);

BENCHMARK_MAIN();