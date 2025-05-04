#include "fxsys.h"

sysinfo _sysinfo;

void init_system_info()
{
#if FX_PLATFORM_WINDOWS
        /* Get page size on Windows */
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        _sysinfo.page_size = si.dwPageSize;
#else
        /* Get page size on Unix-like systems */
        _sysinfo.page_size = (size_t)sysconf(_SC_PAGESIZE);
#endif
}

