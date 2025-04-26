#include <stdio.h>
#include <stdlib.h>

#include "fxpool.h"
#include "fxlog.h"
#include "util.h"
#include "fxsys.h"



uchar* fxpool_aligned_alloc(size_t size, uchar alignment, fx_pool* mp) 
{
        /* Small allocation (aligned heap memory) */
        mp->_pool_size = size;

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


fx_error fxpool_create(size_t each_blk_size, data_unit unit, uint_fast32_t total_blk, uchar align, fx_pool* mp) 
{
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
        if (aligned_size != each_blk_size) 
                pr_warn("fxpool_create: memory block size %zu rounded up to %zu to match alignment %u.\n", each_blk_size, 
                                aligned_size, align);


        mp->_total_blk = total_blk;
        mp->_each_blk_size = each_blk_size;
        
        /* Pool Allocation */
        uchar* mem_start = fxpool_aligned_alloc(TO_BYTES(each_blk_size, unit) * total_blk, align, mp);
        if(mem_start == NULL)
        {
                pr_err("fxpool_create: memory allocation failed. Unable to allocate %zu bytes with alignment %u.\nStdError: %s\n",
                                TO_BYTES(each_blk_size, unit), align, strerror(errno));
                return FX_RES_MEMORY;
        }

        mp->_free_blk = 0;
        mp->_next_blk = mp->_mem_start;

        return FX_RES_OK;
}

void fxpool_destroy()
{

}

void fxpool_create_large_pool()
{

}
