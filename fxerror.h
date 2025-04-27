#ifndef __FX_ERROR__
#define __FX_ERROR__

/*
 * Enum fx_error defines various error codes
 * that can occur in the system. Each code represents
 * a different category of failure or exceptional condition.
 *
 * Members:
 * - FX_RES_OK:             No error; operation successful.
 * - FX_RES_FAIL:           General failure.
 * - FX_RES_IO:             Input/Output error occurred.
 * - FX_RES_LIMIT:          A resource or operational limit was exceeded.
 * - FX_RES_MEMORY:         Memory allocation failed or insufficient memory.
 * - FX_RES_RESOURCE:       Other resource-related failure.
 * - FX_RES_UNIMPL:         Requested feature is not implemented.
 * - FX_RES_COMMIT_LIMIT:   Commit limit for resources reached.
 * - FX_RES_PARAM:          Invalid parameter passed to function.
 * - FX_RES_ALIGNED:        Memory alignment error.
 */
typedef enum fx_error 
{
        FX_RES_OK,
        FX_RES_FAIL,
        FX_RES_IO,
        FX_RES_LIMIT,
        FX_RES_MEMORY,
        FX_RES_RESOURCE,
        FX_RES_UNIMPL,
        FX_RES_COMMIT_LIMIT,
        FX_RES_PARAM,
        FX_RES_ALIGNED,
} fx_error;


/*
 * fx_error_str - Convert an fx_error code into a human-readable string.
 *
 * @code: The fx_error code to be converted.
 *
 * Returns a constant string representing the given error code.
 * Useful for logging, debugging, and error reporting.
 */
const char* fx_error_str(fx_error code);

#endif // __FX_ERROR__

