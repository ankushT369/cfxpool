//BSD 2-Clause License
//
//Copyright (c) 2025, Ankush
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:
//
//1. Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
//2. Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef __FX_ERROR__
#define __FX_ERROR__

#include "fxtypes.h"

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


typedef struct {
    bool ok;        // true if the result is valid
    fx_error err;   // only valid if ok == false
    u64 value;      // only valid if ok == true
} fx_result_u64;

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

