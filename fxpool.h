#ifndef __FX_POOL__
#define __FX_POOL__

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <stdlib.h>

#include "fxerror.h"

typedef unsigned char uchar;

extern uint_fast32_t __fxpool__;

/*
 * Enum alignment_t defines different memory alignment options used in 
 * memory allocation functions to optimize access speed and prevent 
 * misalignment errors.
 * 
 * Members:
 * - SYS_DEF:   System Default alignment
 * - A4:        4-byte alignment
 * - A8:        8-byte alignment
 * - A16:       16-byte alignment
 * - A32:       32-byte alignment
 * - A64:       64-byte alignment
 * 
 * These values ensure that allocated memory is aligned to appropriate 
 * boundaries, improving memory access performance.
 */
typedef enum alignment_t
{
        SYS_DEF = sizeof(void*),        // System Default alignment
        A4      = 4,                    // 4-byte alignment
        A8      = 8,                    // 8-byte alignment
        A16     = 16,                   // 16-byte alignment
        A32     = 32,                   // 32-byte alignment
        A64     = 64,                   // 64-byte alignment
} alignment_t;


typedef enum data_unit
{
        B       = 0,
        KB      = 10,
        MB      = 20,
        GB      = 30,
} data_unit;

static const char* unit_strings[] = {"bytes", "KB", "MB", "GB",};

/*
 * Struct pool_alloc is used to store the state of the 
 * fixed-size memory pool allocator.
 *
 * Members:
 * - _total_blk:        Total number of blocks in the memory pool.
 * - _each_blk_size:    Size (in bytes) of each block in the pool.
 * - _free_blk:         Number of currently available (free) blocks.
 * - _initalized_blk:   Number of blocks that have been initialized so far.
 * - _pool_size:        Total size (in bytes) of the memory pool allocated. This may include
 *                      the extra space used for alignment or any padding.
 * - _mem_start:        Pointer to the beginning of the allocated memory pool.
 * - _next_blk:         Pointer to the next free block in the pool. It keeps track of the 
 *                      next available memory block for allocation.
 * - _unit:             Data unit type that determines the granularity or unit of allocation
 *                      (e.g., bytes, kilobytes, etc.). This helps with the allocation logic 
 *                      based on the requested data unit.
 * -_resize:            This value defines whether to resize if all blocks are allocated. 
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
        uint_fast8_t    _resize;
} fx_pool;


/* basic apis */
fx_error fxpool_create(size_t, data_unit, uint_fast32_t, uchar, fx_pool*);
fx_error fxpool_destroy(fx_pool*);
void* fxpool_alloc(fx_pool*);
fx_error fxpool_dealloc(void*, fx_pool*);
fx_error fxpool_reset(fx_pool*);

/* more advanced apis */
void fxpool_merge();
void fxpool_transfer();
void fxpool_copy();

/* debugging utility */
void fxpool_log(fx_pool*);

/* apis for large memory pools */
fx_error fxpool_create_large_pool();
void fxpool_destroy_large_pool();

#ifdef __cplusplus
}
#endif

#endif // __FM_POOL__
