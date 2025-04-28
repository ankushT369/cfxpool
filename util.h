#ifndef __UTIL_H__
#define __UTIL_H__

#include <inttypes.h>

#include "fxpool.h"

typedef unsigned char uchar;

#ifdef __GNUC__        // For GCC and Clang (Linux, macOS, etc.)
        #define likely(x)   __builtin_expect(!!(x), 1)
        #define unlikely(x) __builtin_expect(!!(x), 0)
#elif defined(_MSC_VER)  // For MSVC (Windows)
        #define likely(x)   (__assume(x), (x))
        #define unlikely(x) (__assume(!(x)), (x)) 
#else  // For other compilers
        #define likely(x)   (x)
        #define unlikely(x) (x)
#endif


// Macro to set the 0th bit to 1
#define SET_BIT(bf)   ((bf) |= 1U)

// Macro to check if the 0th bit is set (1) or not (0)
#define CHECK_BIT(bf) (((bf) & 1U) != 0)

// Macro to clear the 0th bit (set it to 0)
#define CLEAR_BIT(bf) ((bf) &= ~1U)

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
