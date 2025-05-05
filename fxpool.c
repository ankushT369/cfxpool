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


#include <stdio.h>
#include <stdlib.h>

#include "fxpool.h"
#include "fxlog.h"
#include "fxsys.h"
#include "util.h"


static const char* unit_strings[] = { "B", "KB", "MB", "GB" };

void fxpool_log(fx_pool* pool)
{
        if (!pool) 
        {
                pr_err("Error: NULL pointer passed to fxpool_log\n");
                return;
        }
    
        pr_debug("fx_pool Structure:\n");

        pr_debug("   total_blk:            %" PRIuFAST32 "\n", pool->total_blk);
        pr_debug("   each_blk_size:        %" PRIuFAST32 " %s\n", pool->each_blk_size, unit_strings[pool->unit / 10]);
        pr_debug("   free_blk:             %" PRIuFAST32 "\n", pool->nr_free_blk);
        pr_debug("   initalized_blk:       %" PRIuFAST32 "\n", pool->initalized_blk);
        pr_debug("   pool_size(bytes):     %" PRIuFAST64 "\n", pool->pool_size);

        pr_debug("   mem_start:            %p\n", (void*)pool->mem_start_addr);
        pr_debug("   next_blk:             %p\n", (void*)pool->next_blk_addr);

        pr_debug("   alignment:            %" PRIuFAST8 "\n", pool->alignment);
}

void init_fxpool(fx_pool* pool)
{
        if (!pool) 
        {
                pr_err("Error: NULL pointer passed to init_fxpool\n");
                return;
        }

        pool->set_pool          = 0;

        pool->total_blk         = 0; 
        pool->each_blk_size     = 0; 
        pool->nr_free_blk       = 0;
        pool->initalized_blk    = 0;
        pool->pool_size         = 0;

        pool->mem_start_addr    = NULL;
        pool->next_blk_addr     = NULL;

        pool->resize            = false;

        pool->alignment         = SYS_DEF;
        pool->unit              = B;
}

void __register_pool(fx_pool* mp)
{
        /* Checks error before creating a pool */
        SET_BIT(mp->set_pool);
}

void __unregister_pool(fx_pool* mp)
{
        mp->set_pool            = 0;
}

__pool* __fxpool_create_large_pool(size_t size)
{
#ifdef FX_PLATFORM_WINDOWS
        return (__pool*)VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
        return (__pool*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif
return NULL;
}

int __fxpool_destroy_large_pool(fx_pool* mp)
{
#ifdef FX_PLATFORM_WINDOWS
        return VirtualFree((void*)mp->mem_start_addr, 0, MEM_RELEASE);
#else
        return munmap((void*)mp->mem_start_addr, mp->pool_size);
#endif
}

fx_error fxpool_create_large_pool(size_t each_blk_size, data_unit_t unit, u32 total_blk, fx_pool* mp, smode_t mode)
{
        /* Register before creating a pool */
        __register_pool(mp);

        if (unlikely(each_blk_size == 0 || each_blk_size > (1ULL << 48))) 
                return FX_RES_BLK_SIZE;

        if (unlikely(total_blk == 0)) 
                return FX_RES_TOTAL_BLK;

        if ((mode & (FXPOOL_SIZE_AUTO_GROW | FXPOOL_SIZE_CUSTOM)) == mode || 
                (mode & (FXPOOL_SIZE_AUTO_GROW | FXPOOL_SIZE_DEFAULT)) == mode)
                mp->resize = 1;
        
        __pool* mem_addr = __fxpool_create_large_pool((each_blk_size << unit) * total_blk);

        if (!mem_addr)
                return FX_RES_MEMORY;

        mp->each_blk_size       = each_blk_size;
        mp->total_blk           = total_blk;
        mp->nr_free_blk         = total_blk;
        mp->pool_size           = (each_blk_size << unit) * total_blk;
        mp->initalized_blk      = 0;
        mp->alignment           = A4K;
        mp->unit                = unit;

        mp->mem_start_addr      = mem_addr;
        mp->next_blk_addr       = mem_addr;

        return FX_RES_OK;
}

fx_error fxpool_destroy_large_pool(fx_pool* mp)
{               
        if (__fxpool_destroy_large_pool(mp) == 0)
                return FX_RES_OK;
        else
                return FX_RES_MEMORY;
}

__pool* __fxpool_aligned_alloc(size_t size, align_t alignment)
{
#ifdef FX_PLATFORM_WINDOWS
        return(__pool*)_aligned_malloc(size, alignment);
#elif defined(FX_PLATFORM_UNIX)
        void* ptr = NULL;
        if (posix_memalign(&ptr, alignment, size) != 0)
                return NULL;
        return (__pool*)ptr;
#elif defined(FX_PLATFORM_C11)
        return (__pool*)aligned_alloc(alignment, size);
#else
        /* Manual Fallback */
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

void __fxpool_aligned_free(__pool* ptr)
{
        if (!ptr)
                return;
#ifdef FX_PLATFORM_WINDOWS 
        _aligned_free(ptr);
#elif defined(FX_PLATFORM_UNIX)
        free(ptr);  // Standard free
#elif defined(FX_PLATFORM_C11)
        free(ptr);  // Same for aligned_alloc/free
#else
        free(((void**)ptr)[-1]);  // Fallback to manual free
#endif
}

__pool* __fxpool_create(u64 size, align_t align)
{
        return __fxpool_aligned_alloc(size, align);
}

/*
  * Create new block.
  * Create a new big chunk of memory block, from which user allocation will be
  * taken from.
  */
fx_error fxpool_create(size_t each_blk_size, data_unit_t unit, u32 total_blk, align_t align, fx_pool* mp, smode_t mode) 
{
        if (unlikely(!mp)) 
                return FX_RES_PARAM;

        if (mp->pool_size != 0)
                return FX_RES_FAIL;

        if (MB((each_blk_size << unit) * total_blk) >= MB((u64)64))
                return fxpool_create_large_pool(each_blk_size, unit, total_blk, mp, mode);
        
        /* Register before creating a pool */
        __register_pool(mp);

        if (unlikely(!__is_aligned(align))) 
                return FX_RES_ALIGNED;

        if (unlikely(each_blk_size == 0 || each_blk_size > (1ULL << 48))) 
                return FX_RES_BLK_SIZE;

        if (unlikely(total_blk == 0)) 
                return FX_RES_TOTAL_BLK;

        if (unlikely(!CHECK_ARCH_ALIGNMENT(align)))
                return FX_RES_ARCH_ALIGNMENT;

        if ((mode & (FXPOOL_SIZE_AUTO_GROW | FXPOOL_SIZE_CUSTOM)) == mode || 
                (mode & (FXPOOL_SIZE_AUTO_GROW | FXPOOL_SIZE_DEFAULT)) == mode)
                mp->resize = 1;

        /* Align the block size if not aligned */
        u64 aligned_size = __get_aligned_size(each_blk_size, unit, align);

        mp->each_blk_size       = aligned_size;
        mp->total_blk           = total_blk;
        mp->nr_free_blk         = total_blk;
        mp->pool_size           = aligned_size * total_blk;
        mp->initalized_blk      = 0;

        if (align == SYS_DEF)
                mp->alignment   = FX_ARCH_PREFERRED_ALIGN;
        else
                mp->alignment   = align;

        mp->unit                = unit;

        /* Create the memory pool and allocate */
        __pool* mem_addr = __fxpool_create(mp->pool_size, align);

        if (unlikely(!mem_addr))
                return FX_RES_MEMORY;

        mp->mem_start_addr      = mem_addr;
        mp->next_blk_addr       = mem_addr;

        return FX_RES_OK;
}

/*
  * Destroy the pool clean all the allocated memory.
  */
fx_error fxpool_destroy(fx_pool* mp)
{
        if (unlikely(!mp)) 
                return FX_RES_PARAM;

        if (unlikely(!CHECK_BIT(mp->set_pool))) 
                return FX_RES_FAIL;

        if (MB(mp->pool_size) >= MB((u64)64)) 
        {
                if (fxpool_destroy_large_pool(mp) == FX_RES_MEMORY)
                        return FX_RES_MEMORY;
                else
                {
                        mp->mem_start_addr = NULL;
                        return FX_RES_OK;
                }
        }
        else if (mp->mem_start_addr) 
        {
                __fxpool_aligned_free(mp->mem_start_addr);
                mp->mem_start_addr = NULL;
        }

        __unregister_pool(mp);
        init_fxpool(mp);

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
        if (unlikely(!mp)) 
                return NULL;

        if (unlikely(!CHECK_BIT(mp->set_pool))) 
                return NULL;

        if (mp->initalized_blk < mp->total_blk) 
        {
                u32* p = (u32*)__addr_from_index(mp->initalized_blk, mp);
                *p = mp->initalized_blk + 1;
                mp->initalized_blk++;
        }

        void* ret_addr = NULL;
        if (mp->nr_free_blk > 0)
        {
                ret_addr = (void*)mp->next_blk_addr;
                --mp->nr_free_blk;
                if (mp->nr_free_blk != 0) 
                        mp->next_blk_addr = __addr_from_index(*((u32*)mp->next_blk_addr), mp);
                else
                        mp->next_blk_addr = NULL;
        }

        return ret_addr;
}

 /*
  * Dellocate the block.
  */
fx_error fxpool_dealloc(void* ptr, fx_pool* mp)
{
        if (unlikely(!mp)) 
                return FX_RES_PARAM;

        if (unlikely(!ptr)) 
                return FX_RES_PARAM;

        if (unlikely(!CHECK_BIT(mp->set_pool))) 
                return FX_RES_FAIL;

        if (mp->next_blk_addr) 
        {
                (*(u32*)ptr) = __index_from_addr(mp->next_blk_addr, mp);
                mp->next_blk_addr = (__pool*)ptr;
        } else 
        {
                (*(u32*)ptr) = mp->total_blk;
                mp->next_blk_addr = (__pool*)ptr;
        }
        ++mp->nr_free_blk;

        return FX_RES_OK;
}

/*
  * Reset the pool to the state when it was created.
  */
fx_error fxpool_clear(fx_pool* mp)
{
        if (unlikely(!mp)) 
                return FX_RES_PARAM;

        mp->total_blk           = 0; 
        mp->each_blk_size       = 0; 
        mp->nr_free_blk         = 0;
        mp->initalized_blk      = 0;

        mp->next_blk_addr       = mp->mem_start_addr;

        return FX_RES_OK;
}

fx_error fxpool_access(u64 index)
{
       return FX_RES_OK; 
}

u64 fxpool_capacity(fx_pool* mp)
{
        if (unlikely(!mp))
                return 0;
        else if (unlikely(!CHECK_BIT(mp->set_pool)))
                return 0;
        else
                return mp->pool_size;
}

fx_error fxpool_copy_block(void* block, fx_pool* dst_mp)
{
        if (unlikely(!dst_mp)) 
                return FX_RES_PARAM;

        if (unlikely(!block)) 
                return FX_RES_PARAM;

        block = fxpool_alloc(dst_mp);

        return FX_RES_OK;
}
