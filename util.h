#ifndef __UTIL__
#define __UTIL__

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

#define CHECK_ARCH_ALIGNMENT(alignment)                  \
    ((alignment) != 0 &&                                \
     ((alignment) & ((alignment) - 1)) == 0 &&          \
     ((alignment) & (sizeof(void *) - 1)) == 0)


static inline u32 __is_power_of_2(u8 n)
{
    return (n > 0) && ((n & (n - 1)) == 0);
}

static inline u32 __is_aligned(u8 n) 
{
    return __is_power_of_2(n) && (n >= 4 && n <= 64);
}

static inline u8* __addr_from_index(u8 index, fx_pool* fp)
{
        return fp->mem_start_addr + (index * fp->each_blk_size);
}

static inline u32 __index_from_addr(const u8* addr, fx_pool* fp)
{
        return (((uint_fast32_t)(addr - fp->mem_start_addr)) / fp->each_blk_size);
}

static inline u64 __get_aligned_size(size_t size, data_unit unit, size_t alignment)
{
        return ((size << unit) + alignment - 1) & ~(alignment - 1);
}
static inline size_t __round_up_to_page(size_t size, size_t page_size)
{
    return (size + page_size - 1) & ~(page_size - 1);
}


#endif // __UTIL__
