#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "fxpool.h"
#include "fxlog.h"
#include "util.h"
#include "fxsys.h"


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

        // Printing addresses of pointers
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

        pool->_total_blk        = 0; 
        pool->_each_blk_size    = 0; 
        pool->_free_blk         = 0;
        pool->_initalized_blk   = 0;
        pool->_pool_size        = 0;
        pool->_mem_start        = NULL;
        pool->_next_blk         = NULL;
        pool->_unit             = B;
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

        /*
        if (_sysinfo.page_size == 0)
                init_system_info();  // Ensure page size is known

        if (size >= _sysinfo.page_size) 
        {
                // Big allocation (use mmap or VirtualAlloc) 
                *real_alloc_size = round_up_to_page(size, _sysinfo.page_size);
        
                #if defined(_WIN32) || defined (_WIN64)
                        return VirtualAlloc(NULL, *real_alloc_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
                #else
                        void* ptr = mmap(NULL, *real_alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                        return (ptr == MAP_FAILED) ? NULL : ptr;
                #endif
        } 
        else 
        {
        }
        */
}

void fxpool_aligned_free(uchar* ptr)
{
        if (ptr == NULL)
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

fx_error fxpool_create(size_t each_blk_size, data_unit unit, uint_fast32_t total_blk, uchar align, fx_pool* mp) 
{
        if (!mp)
        {
                pr_err("fxpool_create: pool pointer is NULL. Cannot create pool.\n");
                return FX_RES_PARAM;
        }

        if (!is_aligned(align))
        {
                pr_err("fxpool_create: alignment value %u is not valid. Memory alignment check failed.\n", align);
                return FX_RES_ALIGNED;
        }

        if (each_blk_size == 0 || each_blk_size > (1ULL << 48)) 
        {
                pr_err("fxpool_create: block size %zu is too large or zero. Possible overflow or invalid input.\n", each_blk_size);
                return FX_RES_PARAM;
        }

        if (total_blk == 0)
        {
                pr_err("fxpool_create: total number of blocks is zero. Invalid input, pool cannot be created with zero blocks.\n");
                return FX_RES_PARAM;
        }

        size_t aligned_size = align_memory(each_blk_size, align);

        mp->_total_blk          = total_blk;
        mp->_each_blk_size      = aligned_size;
        
        /* Pool Allocation */
        uchar* mem_start = fxpool_aligned_alloc(TO_BYTES(each_blk_size, unit) * total_blk, align);
        if (!mem_start)
        {
                pr_err("fxpool_create: memory allocation failed. Unable to allocate %zu bytes with alignment %u.\nStdError: %s\n",
                                TO_BYTES(each_blk_size, unit) * total_blk, align, strerror(errno));
                return FX_RES_MEMORY;
        }

        mp->_free_blk           = total_blk;
        mp->_next_blk           = mem_start;
        mp->_pool_size          = TO_BYTES(each_blk_size, unit) * total_blk;
        mp->_mem_start          = mem_start;
        mp->_unit               = unit;

        return FX_RES_OK;
}

fx_error fxpool_destroy(fx_pool* mp)
{
        if (!mp)
        {
                pr_err("fxpool_destroy: pool pointer is NULL. Cannot destroy a non-existent pool.\n");
                return FX_RES_PARAM;
        }

        if (mp->_mem_start)
        {

                fxpool_aligned_free(mp->_mem_start);
                mp->_mem_start = NULL;
        }
        else
                pr_warn("fxpool_destroy: pool memory already NULL. Nothing to free.\n");

        return FX_RES_OK;
}

void* fxpool_alloc(fx_pool* mp)
{
        if (!mp)
        {
                pr_err("fxpool_alloc: pool pointer is NULL or an unknown memory address. Cannot allocate a non-existent memory location.\n");
                return NULL;
        }

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
                if (mp->_free_blk != 0) {
                        mp->_next_blk = addr_from_index(*((uint_fast32_t*)mp->_next_blk), mp);
                }
                else {
                        mp->_next_blk = NULL;
                }
        }

        if (!ret)
        {
                pr_err("fxpool_alloc: No free blocks available in the memory pool or in that memory location. Allocation failed.\n");;
                return NULL;
        }

        return ret;
}

fx_error fxpool_dealloc(void* ptr, fx_pool* mp)
{
        if (!mp)
        {
                pr_err("fxpool_dealloc: pool pointer is NULL or an unknown memory address. Cannot dellocate a non-existent memory location.\n");
                return FX_RES_PARAM;
        }

        if (mp->_next_blk != NULL)
        {
                (*(uint_fast32_t*)ptr) = index_from_addr(mp->_next_blk, mp);
                mp->_next_blk = (uchar*)ptr;
        }
        else 
        {
                (*(uint_fast32_t*)ptr) = mp->_total_blk;
                mp->_next_blk = (uchar*)ptr;
        }
        ++mp->_free_blk;

        return FX_RES_OK;
}

fx_error fxpool_create_large_pool()
{
        return FX_RES_UNIMPL;
}
