#include <benchmark/benchmark.h>
#include "../fxpool.h"
#include "../fxerror.h"
#include <iostream>

static void BM_fxpool_alloc(benchmark::State& state)
{
        fx_pool pool = {0};
        size_t ebs = state.range(0);
        data_unit_t unit = B;
        uint_fast32_t tblk = state.range(1);
        align_t align = A64;

        fx_error err = fxpool_create_large_pool(ebs, unit, tblk, &pool, FXPOOL_SIZE_CUSTOM);
        
        for (auto _ : state) {
                void* ptr = fxpool_alloc(&pool);
                benchmark::DoNotOptimize(ptr);
                fxpool_dealloc(ptr, &pool);
        }

        fxpool_destroy_large_pool(&pool);
}
BENCHMARK(BM_fxpool_alloc)
->Args({64, 100000}) // 6.10 MB
->Args({128, 50000}) // 6.10 MB
->Args({1024, 5000})  // 4.88 MB
->Args({4096, 1000})  // 3.91 MB
->Args({1048576, 10})  // 10.00 MB
->Args({2097152, 10})  // 20.00 MB
->Args({8192, 100000})  // 781.25 MB
->Args({32768, 50000}); // 1562.50 MB



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
BENCHMARK(BM_malloc)
->Args({64, 100000}) // 6.10 MB
->Args({128, 50000}) // 6.10 MB
->Args({1024, 5000})  // 4.88 MB
->Args({4096, 1000})  // 3.91 MB
->Args({1048576, 10})  // 10.00 MB
->Args({2097152, 10})  // 20.00 MB
->Args({8192, 100000})  // 781.25 MB
->Args({32768, 50000}); // 1562.50 MB

BENCHMARK_MAIN();
