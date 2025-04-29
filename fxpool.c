#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "fxpool.h"
#include "fxlog.h"
#include "util.h"
#include "fxsys.h"

uint_fast32_t __fxpool__ = 0;

void fxpool_log(fx_pool* pool)
{
        if (!pool) 
        {
                pr_err("Error: NULL pointer passed to fxpool_log\n");
                return;
        }
    
        pr_debug("fx_pool Structure:\n");
        pr_debug("   total_blk:            %" PRIuFAST32 "\n", pool->_total_blk);
        pr_debug("   each_blk_size:        %" PRIuFAST32 " %s\n", pool->_each_blk_size, unit_strings[pool->_unit / 10]);
        pr_debug("   free_blk:             %" PRIuFAST32 "\n", pool->_free_blk);
        pr_debug("   initalized_blk:       %" PRIuFAST32 "\n", pool->_initalized_blk);
        pr_debug("   pool_size(bytes):     %" PRIuFAST64 "\n", pool->_pool_size);
        pr_debug("   mem_start:            0x%p\n", (void*)pool->_mem_start);
        pr_debug("   next_blk:             0x%p\n", (void*)pool->_next_blk);
}

void init_fxpool(fx_pool* pool)
{
        if (!pool) 
        {
                pr_err("Error: NULL pointer passed to init_fxpool\n");
                return;
        }

        pool->_total_blk = 0; 
        pool->_each_blk_size = 0; 
        pool->_free_blk = 0;
        pool->_initalized_blk = 0;
        pool->_pool_size = 0;
        pool->_mem_start = NULL;
        pool->_next_blk = NULL;
        pool->_unit = B;
}

uchar* fxpool_aligned_alloc(size_t size, uchar alignment) 
{
#if defined(_MSC_VER)
        return _aligned_malloc(size, alignment);
#elif defined(__unix__) || defined(__APPLE__)
        void* ptr = NULL;
        if (posix_memalign(&ptr, alignment, size) != 0)
                return NULL;
        return (uchar*)ptr;
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
        return aligned_alloc(alignment, size);
#else
        /* Manual fallback */
        void* p1;
        void** p2;
        size_t offset = alignment - 1 + sizeof(void*);
        p1 = malloc(size + offset);
        if (p1 == NULL) return NULL;
        p2 = (void**)(((uintptr_t)p1 + offset) & ~(alignment - 1));
        p2[-1] = p1;
        return p2;
#endif
}

void fxpool_aligned_free(uchar* ptr)
{
        if (!ptr)
                return;
#if defined(_MSC_VER)
        _aligned_free(ptr);
#elif defined(__unix__) || defined(__APPLE__)
        free(ptr);  // Standard free
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
        free(ptr);  // Same for aligned_alloc/free
#else
        free(((void**)ptr)[-1]);  // Fallback to manual free
#endif
}

/*
  * Create new block.
  * Create a new big chunk of memory block, from which user allocation will be
  * taken from.
  */
fx_error fxpool_create(size_t each_blk_size, data_unit unit, uint_fast32_t total_blk, uchar align, fx_pool* mp) 
{
        SET_BIT(__fxpool__);

        if (unlikely(!mp)) 
                return FX_RES_PARAM;

        if (unlikely(!is_aligned(align))) 
                return FX_RES_ALIGNED;

        if (unlikely(each_blk_size == 0 || each_blk_size > (1ULL << 48))) 
                return FX_RES_BLK_SIZE;

        if (unlikely(total_blk == 0)) 
                return FX_RES_TOTAL_BLK;

        if (unlikely(!CHECK_ARCH_ALIGNMENT(align)))
                return FX_RES_ARCH_ALIGNMENT;

        size_t aligned_size = align_memory(each_blk_size, align);

        mp->_total_blk = total_blk;
        mp->_each_blk_size = aligned_size;
        
        /* Pool Allocation */
        uchar* mem_start = fxpool_aligned_alloc(TO_BYTES(aligned_size, unit) * total_blk, align);
        if (unlikely(!mem_start))
                return FX_RES_MEMORY;

        mp->_free_blk           = total_blk;
        mp->_next_blk           = mem_start;
        mp->_pool_size          = TO_BYTES(aligned_size, unit) * total_blk;
        mp->_mem_start          = mem_start;
        mp->_unit               = unit;

        return FX_RES_OK;
}

/*
  * Destroy the pool clean all the allocated memory.
  */
fx_error fxpool_destroy(fx_pool* mp)
{
        if (unlikely(!CHECK_BIT(__fxpool__))) 
                return FX_RES_FAIL;

        if (unlikely(!mp)) 
                return FX_RES_PARAM;

        if (mp->_mem_start) 
        {
                fxpool_aligned_free(mp->_mem_start);
                mp->_mem_start = NULL;
        }

        init_fxpool(mp);
        CLEAR_BIT(__fxpool__);
        return FX_RES_OK;
}

 /*
  * Allocate memory chunk for user from available blocks.
  * If no space is available in all the blocks
  * a new block might be created (depending on whether the pool is allowed
  * to resize).
  */
void* fxpool_alloc(fx_pool* mp)
{
        if (unlikely(!CHECK_BIT(__fxpool__))) 
                return NULL;

        if (unlikely(!mp)) 
                return NULL;

        if (mp->_initalized_blk < mp->_total_blk) 
        {
                uint_fast32_t* p = (uint_fast32_t*)addr_from_index(mp->_initalized_blk, mp);
                *p = mp->_initalized_blk + 1;
                mp->_initalized_blk++;
        }

        void* ret = NULL;
        if (mp->_free_blk > 0)
        {
                ret = (void*)mp->_next_blk;
                --mp->_free_blk;
                if (mp->_free_blk != 0) 
                        mp->_next_blk = addr_from_index(*((uint_fast32_t*)mp->_next_blk), mp);
                else
                        mp->_next_blk = NULL;
        }

        return ret;
}

 /*
  * Dellocate the block.
  */
fx_error fxpool_dealloc(void* ptr, fx_pool* mp)
{
        if (unlikely(!CHECK_BIT(__fxpool__))) 
                return FX_RES_FAIL;

        if (unlikely(!mp)) 
                return FX_RES_PARAM;

        if (unlikely(!ptr)) 
                return FX_RES_PARAM;

        if (mp->_next_blk) 
        {
                (*(uint_fast32_t*)ptr) = index_from_addr(mp->_next_blk, mp);
                mp->_next_blk = (uchar*)ptr;
        } else 
        {
                (*(uint_fast32_t*)ptr) = mp->_total_blk;
                mp->_next_blk = (uchar*)ptr;
        }
        ++mp->_free_blk;

        return FX_RES_OK;
}

/*
  * Reset the pool to the state when it was created.
  */
fx_error fxpool_reset(fx_pool* mp)
{
        if (unlikely(!mp)) 
                return FX_RES_PARAM;

        mp->_total_blk          = 0; 
        mp->_each_blk_size      = 0; 
        mp->_free_blk           = 0;
        mp->_initalized_blk     = 0;
        mp->_next_blk           = mp->_mem_start;

        return FX_RES_OK;
}

