#ifndef __FX_ERROR__
#define __FX_ERROR__

/*
 * Enum fx_error defines error codes for various
 * failures or exceptional situations in the system.
 *
 * Members:
 * - FX_RES_OK:                 No error; operation successful.
 * - FX_RES_FAIL:               General failure.
 * - FX_RES_IO:                 Input/Output error occurred.
 * - FX_RES_LIMIT:              A limit (like memory or resource limit) was exceeded.
 * - FX_RES_MEMORY:             Memory allocation failed or not enough memory available.
 * - FX_RES_RESOURCE:           Failure related to other system resources.
 * - FX_RES_UNIMPL:             Requested feature is not implemented yet.
 * - FX_RES_COMMIT_LIMIT:       Resource commit limit (e.g., memory commit) reached.
 * - FX_RES_PARAM:              Invalid parameter provided to a function.
 * - FX_RES_ALIGNED:            Memory alignment check failed.
 * - FX_RES_BLK_SIZE:           Invalid block size specified for memory pool.
 * - FX_RES_TOTAL_BLK:          Invalid number of blocks specified for memory pool.
 * - FX_RES_ARCH_ALIGNMENT:     Alignment requirement not met for the target architecture 
 *                              (e.g., not a power of two or not a multiple of sizeof(void *)).
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
        FX_RES_BLK_SIZE,
        FX_RES_TOTAL_BLK,
        FX_RES_ARCH_ALIGNMENT,
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

