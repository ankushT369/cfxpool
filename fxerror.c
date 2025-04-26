#include "fxerror.h"

const char* fx_error_str(fx_error code)
{
    switch (code)
    {
    case FX_RES_OK:            return "OK";
    case FX_RES_FAIL:          return "Failure";
    case FX_RES_IO:            return "I/O Error";
    case FX_RES_LIMIT:         return "Limit Exceeded";
    case FX_RES_MEMORY:        return "Out of Memory";
    case FX_RES_RESOURCE:      return "Resource Error";
    case FX_RES_UNIMPL:        return "Unimplemented";
    case FX_RES_COMMIT_LIMIT:  return "Commit Limit Reached";
    case FX_RES_PARAM:         return "Invalid Parameter";
    case FX_RES_ALIGNED:       return "Invalid Alignment";
    default:                   return "Unknown Error";
    }
}

