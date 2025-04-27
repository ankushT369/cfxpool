#ifndef __FX_POOL__
#define __FX_POOL__

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <stdlib.h>

#include "fxerror.h"

typedef unsigned char uchar;

/*
 * Enum alignment_t defines different memory alignment options used in 
 * memory allocation functions to optimize access speed and prevent 
 * misalignment errors.
 * 
 * Members:
 * - DEFAULT:    Default alignment (typically 4 bytes).
 * - ALIGN_4:    4-byte alignment (2^2).
 * - ALIGN_8:    8-byte alignment (2^3).
 * - ALIGN_16:   16-byte alignment (2^4).
 * - ALIGN_32:   32-byte alignment (2^5).
 * - ALIGN_64:   64-byte alignment (2^6).
 * 
 * These values ensure that allocated memory is aligned to appropriate 
 * boundaries, improving memory access performance.
 */
typedef enum alignment_t
{
        DEFAULT  = 4,   // Default alignment, typically 4 bytes
        ALIGN_4  = (1 << 2), // 4-byte alignment (2^2)
        ALIGN_8  = (1 << 3), // 8-byte alignment (2^3)
        ALIGN_16 = (1 << 4), // 16-byte alignment (2^4)
        ALIGN_32 = (1 << 5), // 32-byte alignment (2^5)
        ALIGN_64 = (1 << 6), // 64-byte alignment (2^6)
} alignment_t;


typedef enum data_unit
{
        B = 0,
        KB = 10,
        MB = 20,
        GB = 30,
        TB = 40,
} data_unit;

static const char* unit_strings[] = {"bytes", "KB", "MB", "GB", "TB"};

/*
 * Struct pool_alloc is used to store the state of the 
 * fixed-size memory pool allocator.
 *
 * Members:
 * - _total_blk:         Total number of blocks in the memory pool.
 * - _each_blk_size:     Size (in bytes) of each block in the pool.
 * - _free_blk:          Number of currently available (free) blocks.
 * - _initalized_blk:    Number of blocks that have been initialized so far.
 * - _pool_size:         Total size (in bytes) of the memory pool allocated. This may include
 *                       the extra space used for alignment or any padding.
 * - _mem_start:         Pointer to the beginning of the allocated memory pool.
 * - _next_blk:          Pointer to the next free block in the pool. It keeps track of the 
 *                       next available memory block for allocation.
 * - _unit:              Data unit type that determines the granularity or unit of allocation
 *                       (e.g., bytes, kilobytes, etc.). This helps with the allocation logic 
 *                       based on the requested data unit.
 * 
 * This structure keeps track of memory usage and allocation status
 * for fast and efficient block-based memory management.
 */typedef struct fx_pool
{
        uint_fast32_t   _total_blk;
        uint_fast32_t   _each_blk_size;
        uint_fast32_t   _free_blk;
        uint_fast32_t   _initalized_blk;
        uint_fast64_t   _pool_size;
        uchar*          _mem_start;
        uchar*          _next_blk; 
        data_unit       _unit;
} fx_pool;


/* basic apis */
fx_error fxpool_create(size_t, data_unit, uint_fast32_t, uchar, fx_pool*);
fx_error fxpool_destroy(fx_pool*);
void* fxpool_alloc(fx_pool*);
fx_error fxpool_dealloc(void*, fx_pool*);

/* more advanced apis */
void fxpool_merge();
void fxpool_transfer();

/* debugging utility */
void fxpool_log(fx_pool*);

/* apis for large memory pools */
fx_error fxpool_create_large_pool();
void fxpool_destroy_large_pool();

#ifdef __cplusplus
}
#endif

#endif // __FM_POOL__
