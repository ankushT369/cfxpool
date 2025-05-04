#ifndef __FX_SYS__
#define __FX_SYS__

#include <stdlib.h>

#include "fxerror.h"

/* Platform detection and macro definitions */
#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
        #define FX_PLATFORM_WINDOWS
#elif defined(__unix__) || defined(__APPLE__)
        #define FX_PLATFORM_UNIX
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
        #define FX_PLATFORM_C11
#else
        #define FX_PLATFORM_UNKNOWN
#endif

// You can now include headers based on the macro
#ifdef FX_PLATFORM_WINDOWS
    #include <windows.h>
#else
    #include <sys/mman.h>
    #include <unistd.h>
#endif

typedef struct sysinfo
{
        size_t page_size;
} sysinfo;

extern sysinfo _sysinfo;

void init_system_info();

#endif // __FX_SYS__
