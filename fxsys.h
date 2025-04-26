#ifndef __FX_SYS__
#define __FX_SYS__

#include <stdlib.h>

/* Platform detection */
#if defined(_WIN32) || defined(_WIN64)
        #include <windows.h>  // Windows headers
#else
        #include <sys/mman.h> // mmap(), munmap()
        #include <unistd.h>   // Unix-like headers
#endif

typedef struct sysinfo
{
        size_t page_size;
} sysinfo;

extern sysinfo _sysinfo;

void init_system_info();

#endif // __FX_SYS__
