#include <benchmark/benchmark.h>
#include "../fxpool.h"

fx_pool pool;
static void BM_fxpool_create(benchmark::State& state)
{
        size_t ebs = 1000;
        data_unit unit = KB;
        uint_fast32_t tblk = 2000;
        uchar align = 8;

        for (auto _ : state) {
                if (state.iterations() == 50) {
                        fx_error err = fxpool_create(ebs, unit, tblk, align, &pool);
                        benchmark::DoNotOptimize(err);
                        fxpool_destroy(&pool);
                }       
        }
}
BENCHMARK(BM_fxpool_create);

static void BM_malloc(benchmark::State& state) 
{
        size_t each_blk_size = 1000 * 1024;
        uint_fast32_t total_blk = 2000;

        for (auto _ : state) {
                if (state.iterations() == 50) {
                        void* ptr = malloc(each_blk_size * total_blk);
                        benchmark::DoNotOptimize(ptr);
                free(ptr);
                }
        }
}
BENCHMARK(BM_malloc);

BENCHMARK_MAIN();

