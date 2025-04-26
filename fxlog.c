#include <stdio.h>
#include <stdarg.h>

void vprint_on_level(unsigned int loglevel, const char *format, va_list args)
{
    vfprintf(stderr, format, args);
}

void print_on_level(unsigned int loglevel, const char *format, ...)
{
    va_list params;
    va_start(params, format);
    vprint_on_level(loglevel, format, params);
    va_end(params);
}


