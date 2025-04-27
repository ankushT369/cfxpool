#include <benchmark/benchmark.h>
#include "../fxpool.h"

static void BM_fxpool_alloc(benchmark::State& state)
{
        fx_pool pool;
        size_t ebs = state.range(0);
        data_unit unit = MB;
        uint_fast32_t tblk = state.range(1);
        uchar align = 8;

        fx_error err = fxpool_create(ebs, unit, tblk, align, &pool);
        for (auto _ : state) {
                void* ptr = fxpool_alloc(&pool);
                benchmark::DoNotOptimize(ptr);
                fxpool_dealloc(ptr, &pool);
        }

        fxpool_destroy(&pool);
}
BENCHMARK(BM_fxpool_alloc)->Args({72, 100})->Args({128, 10}); 


static void BM_malloc(benchmark::State& state) 
{
        size_t each_blk_size = state.range(0);
        uint_fast32_t total_blk = state.range(1);

        for (auto _ : state) {
                void* ptr = malloc(each_blk_size);
                benchmark::DoNotOptimize(ptr);
                free(ptr);
        }
}
BENCHMARK(BM_malloc)->Args({72 * 1024 * 1024, 100})->Args({128, 10});


BENCHMARK_MAIN();

