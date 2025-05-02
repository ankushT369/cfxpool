//BSD 2-Clause License
//
//Copyright (c) 2025, Ankush
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:
//
//1. Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
//2. Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#ifndef __FX_POOL__
#define __FX_POOL__

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>

#include "fxerror.h"
#include "fxtypes.h"

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
 * - A128:      128-byte alignment
 * - A256:      256-byte alignment
 * - A512:      512-byte alignment
 * 
 * These values ensure that allocated memory is aligned to appropriate 
 * boundaries, improving memory access performance.
 */
typedef enum
{
        SYS_DEF = sizeof(void*),        // System Default alignment
        A4      = 4,                    // 4-byte alignment
        A8      = 8,                    // 8-byte alignment
        A16     = 16,                   // 16-byte alignment
        A32     = 32,                   // 32-byte alignment
        A64     = 64,                   // 64-byte alignment
        A128    = 128,                  // 128-byte alignment
        A256    = 256,                  // 256-byte alignment
        A512    = 512,                  // 512-byte alignment
} align_t;


typedef enum
{
        B       = 0,
        KB      = 10,
        MB      = 20,
        GB      = 30,
} data_unit_t;


typedef enum
{
        FXPOOL_SIZE_DEFAULT     = (1 << 0),
        FXPOOL_SIZE_CUSTOM      = (1 << 2),
        FXPOOL_SIZE_AUTO_GROW   = (1 << 4),
} smode_t;

/*
 * Struct pool_alloc is used to store the state of the 
 * fixed-size memory pool allocator.
 *
 * Members:
 * -total_blk:          Total number of blocks in the memory pool.
 * -each_blk_size:      Size (in bytes) of each block in the pool.
 * -free_blk:           Number of currently available (free) blocks.
 * -initalized_blk:     Number of blocks that have been initialized so far.
 * -pool_size:          Total size (in bytes) of the memory pool allocated. This may include
 *                      the extra space used for alignment or any padding.
 * -mem_start:          Pointer to the beginning of the allocated memory pool.
 * -next_blk:           Pointer to the next free block in the pool. It keeps track of the 
 *                      next available memory block for allocation.
 * -unit:               Data unit type that determines the granularity or unit of allocation
 *                      (e.g., bytes, kilobytes, etc.). This helps with the allocation logic 
 *                      based on the requested data unit.
 * -resize:             This value defines whether to resize if all blocks are allocated. 
 * -mode:               Size mode type of the memory pool it define whether to grow automatically. 
 * 
 * This structure keeps track of memory usage and allocation status
 * for fast and efficient block-based memory management.
 */


typedef struct
{
        /* pool set */
        u8              set_pool;

        /* Basic fields */
        u32             total_blk;
        u32             each_blk_size;
        u32             nr_free_blk;
        u32             initalized_blk;
        u64             pool_size;

        /* Pool Data types */
        __pool*         mem_start_addr;
        __pool*         next_blk_addr; 

        /* Auto resize pool */
        bool            resize;

        /* Unit, Alignment and mode of the memory block */
        align_t         alignment;
        data_unit_t     unit;
        smode_t         mode;
} fx_pool;


/* basic apis */
fx_error fxpool_create(size_t, data_unit_t, u32, align_t, fx_pool*, smode_t);
fx_error fxpool_destroy(fx_pool*);
void* fxpool_alloc(fx_pool*);
fx_error fxpool_dealloc(void*, fx_pool*);
fx_error fxpool_clear(fx_pool*);

/* introspects apis */
u64 fxpool_capacity(fx_pool*);

/* more advanced apis */
void fxpool_merge();
void fxpool_transfer_block();
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
