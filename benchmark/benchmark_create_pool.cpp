#include <benchmark/benchmark.h>
#include "../fxpool.h"
#include "../fxerror.h"
#include <iostream>

static void BM_fxpool_creation(benchmark::State& state)
{
    size_t ebs = state.range(0);
    uint_fast32_t tblk = state.range(1);

    for (auto _ : state) {
        fx_pool pool = {0};
        fx_error err = fxpool_create_large_pool(ebs, B, tblk, &pool, FXPOOL_SIZE_CUSTOM);
        benchmark::DoNotOptimize(pool);
        fxpool_destroy_large_pool(&pool);
    }
}
BENCHMARK(BM_fxpool_creation)
->Args({64, 100000}) // 6.10 MB
->Args({128, 50000}) // 6.10 MB
->Args({1024, 5000})  // 4.88 MB
->Args({4096, 1000})  // 3.91 MB
->Args({1048576, 10})  // 10.00 MB
->Args({2097152, 10})  // 20.00 MB
->Args({8192, 100000})  // 781.25 MB
->Args({32768, 50000}); // 1562.50 MB


static void BM_bulk_malloc(benchmark::State& state)
{
    size_t ebs = state.range(0);
    uint_fast32_t tblk = state.range(1);

    for (auto _ : state) {
        std::vector<void*> ptrs;
        ptrs.reserve(tblk);
        for (uint_fast32_t i = 0; i < tblk; ++i) {
            void* ptr = malloc(ebs);
            benchmark::DoNotOptimize(ptr);
            ptrs.push_back(ptr);
        }
        for (void* ptr : ptrs) {
            free(ptr);
        }
    }
}
BENCHMARK(BM_bulk_malloc)
->Args({64, 100000}) // 6.10 MB
->Args({128, 50000}) // 6.10 MB
->Args({1024, 5000})  // 4.88 MB
->Args({4096, 1000})  // 3.91 MB
->Args({1048576, 10})  // 10.00 MB
->Args({2097152, 10})  // 20.00 MB
->Args({8192, 100000})  // 781.25 MB
->Args({32768, 50000}); // 1562.50 MB

BENCHMARK_MAIN();
