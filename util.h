#ifndef __UTIL_H__
#define __UTIL_H__

#include <inttypes.h>

#include "fxpool.h"

typedef unsigned char uchar;

#define TO_BYTES(x, unit) ((x) << (unit))


static inline int is_power_of_2(uint_fast8_t n)
{
    return (n > 0) && ((n & (n - 1)) == 0);
}

static inline int is_aligned(uint_fast8_t n) 
{
    return is_power_of_2(n) && (n >= 4 && n <= 64);
}

static inline uchar* addr_from_index(uint_fast32_t index, fx_pool* fp)
{
        return fp->_mem_start + (index * fp->_each_blk_size);
}

static inline uint32_t index_from_addr(const uchar* addr, fx_pool* fp)
{
        return (((uint_fast32_t)(addr - fp->_mem_start)) / fp->_each_blk_size);
}

static inline uint_fast32_t align_memory(uint_fast32_t size, uint_fast8_t alignment)
{
        return (((size) + (alignment) - 1) & ~((alignment) - 1));
}

static inline size_t round_up_to_page(size_t size, size_t page_size)
{
    return (size + page_size - 1) & ~(page_size - 1);
}


#endif // __UTIL_H__
