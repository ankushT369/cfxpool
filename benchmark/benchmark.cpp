#include <benchmark/benchmark.h>
#include "../fxpool.h"
#include "../fxerror.h"
#include <iostream>
#include <unistd.h>

static void BM_fxpool_alloc(benchmark::State& state)
{
        fx_pool pool;
        size_t ebs = state.range(0);
        data_unit_t unit = KB;
        uint_fast32_t tblk = state.range(1);
        align_t align = A64;

        pool.pool_idx          = NULL;

        pool.total_blk         = 0; 
        pool.each_blk_size     = 0; 
        pool.nr_free_blk       = 0;
        pool.initalized_blk    = 0;
        pool.pool_size         = 0;
        //sleep(100);
        fx_error err = fxpool_create(ebs, unit, tblk, align, &pool, FXPOOL_SIZE_CUSTOM);
        
        std::cout << err << '\n';
        fxpool_log(&pool);
        for (auto _ : state) {
                void* ptr = fxpool_alloc(&pool);
                benchmark::DoNotOptimize(ptr);
                fxpool_dealloc(ptr, &pool);
        }

        fxpool_destroy(&pool);
}
BENCHMARK(BM_fxpool_alloc)->Args({37 * 1024, 1000})->Args({128 * 1024, 10})->Args({49 * 1024, 100}); 


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
BENCHMARK(BM_malloc)->Args({37 * 1024, 1000})->Args({128 * 1024, 10})->Args({49 * 1024, 100});


BENCHMARK_MAIN();

