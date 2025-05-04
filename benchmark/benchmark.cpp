#include <benchmark/benchmark.h>
#include "../fxpool.h"
#include "../fxerror.h"
#include <iostream>
#include <unistd.h>

static void BM_fxpool_destroy(benchmark::State& state)
{
    size_t ebs = state.range(0);
    data_unit_t unit = B;
    uint_fast32_t tblk = state.range(1);
    align_t align = A64;

    for (auto _ : state) {
        fx_pool pool = {0};
        fxpool_create_large_pool(ebs, unit, tblk, &pool, FXPOOL_SIZE_CUSTOM);

        benchmark::DoNotOptimize(pool);

        fxpool_destroy_large_pool(&pool);
    }
}
BENCHMARK(BM_fxpool_destroy)
->Args({13421773, 10})      // 134,217,730 bytes
->Args({80896, 3222})       // 260,489,472 bytes
->Args({5138022, 100})      // 513,802,200 bytes
->Args({55296, 10000})      // 552,960,000 bytes
->Args({1782579, 1000})     // 1,782,579,000 bytes
->Args({3879731, 1000});    // 3,879,731,000 bytes



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
->Args({13421773, 10})      // 134,217,730 bytes
->Args({80896, 3222})       // 260,489,472 bytes
->Args({5138022, 100})      // 513,802,200 bytes
->Args({55296, 10000})      // 552,960,000 bytes
->Args({1782579, 1000})     // 1,782,579,000 bytes
->Args({3879731, 1000});    // 3,879,731,000 bytes


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
->Args({13421773, 10})      // 134,217,730 bytes
->Args({80896, 3222})       // 260,489,472 bytes
->Args({5138022, 100})      // 513,802,200 bytes
->Args({55296, 10000})      // 552,960,000 bytes
->Args({1782579, 1000})     // 1,782,579,000 bytes
->Args({3879731, 1000});    // 3,879,731,000 bytes
//BENCHMARK(BM_fxpool_alloc)->Args({37 * 1024, 1000})->Args({128 * 1024, 10})->Args({49 * 1024, 100}); 

//////////////////////////////////////////////separation/////////////////////////////////////////////////////////////

static void BM_bulk_free(benchmark::State& state)
{
    size_t each_blk_size = state.range(0);
    uint_fast32_t total_blk = state.range(1);

    for (auto _ : state) {
        std::vector<void*> ptrs(total_blk);
        for (uint_fast32_t i = 0; i < total_blk; ++i)
            ptrs[i] = malloc(each_blk_size);

        for (void* ptr : ptrs)
            free(ptr);
    }
}
BENCHMARK(BM_bulk_free)
->Args({13421773, 10})      // 134,217,730 bytes
->Args({80896, 3222})       // 260,489,472 bytes
->Args({5138022, 100})      // 513,802,200 bytes
->Args({55296, 10000})      // 552,960,000 bytes
->Args({1782579, 1000})     // 1,782,579,000 bytes
->Args({3879731, 1000});    // 3,879,731,000 bytes


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
->Args({13421773, 10})      // 134,217,730 bytes
->Args({80896, 3222})       // 260,489,472 bytes
->Args({5138022, 100})      // 513,802,200 bytes
->Args({55296, 10000})      // 552,960,000 bytes
->Args({1782579, 1000})     // 1,782,579,000 bytes
->Args({3879731, 1000});    // 3,879,731,000 bytes

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
->Args({13421773, 10})      // 134,217,730 bytes
->Args({80896, 3222})       // 260,489,472 bytes
->Args({5138022, 100})      // 513,802,200 bytes
->Args({55296, 10000})      // 552,960,000 bytes
->Args({1782579, 1000})     // 1,782,579,000 bytes
->Args({3879731, 1000});    // 3,879,731,000 bytes
//BENCHMARK(BM_malloc)->Args({37 * 1024, 1000})->Args({128 * 1024, 10})->Args({49 * 1024, 100});


BENCHMARK_MAIN();

