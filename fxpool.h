#ifndef __FX_POOL__
#define __FX_POOL__

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <stdlib.h>

#include "fxerror.h"

typedef unsigned char uchar;

typedef enum alignment_t
{
        DEFAULT  = 4,
        ALIGN_4  = (1 << 2),
        ALIGN_8  = (1 << 3),
        ALIGN_16 = (1 << 4),
        ALIGN_32 = (1 << 5),
        ALIGN_64 = (1 << 6),
} alignment_t;

typedef enum data_unit
{
        B = 1,
        KB = 10,
        MB = 20,
        GB = 30,
        TB = 40,
} data_unit;

/*
 * Struct pool_alloc is used to store the state of the 
 * fixed-size memory pool allocator.
 *
 * Members:
 * - _total_blk:         Total number of blocks in the memory pool.
 * - _each_blk_size:     Size (in bytes) of each block in the pool.
 * - _free_blk:          Number of currently available (free) blocks.
 * - _initalized_blk:    Number of blocks that have been initialized so far.
 * - _mem_start:         Pointer to the beginning of the allocated memory pool.
 * - _next_blk:          Pointer to the next free block in the pool.
 * This structure keeps track of memory usage and allocation status
 * for fast and efficient block-based memory management.
 */
typedef struct fx_pool
{
        uint_fast32_t   _total_blk;
        uint_fast32_t   _each_blk_size;
        uint_fast32_t   _free_blk;
        uint_fast32_t   _initalized_blk;
        uint_fast64_t   _pool_size;
        uchar*          _mem_start;
        uchar*          _next_blk; 
} fx_pool;


fx_error fxpool_create(size_t, data_unit, uint_fast32_t, uchar, fx_pool*);
void fxpool_create_large_pool();
void fxpool_alloc();
void fxpool_dealloc();
void fxpool_destroy();
void fxpool_destroy_large_pool();
void fxpool_merge();
void fxpool_transfer();

#ifdef __cplusplus
}
#endif

#endif // __FM_POOL__
