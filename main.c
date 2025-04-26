#include <stdio.h>

#include "fxpool.h"
#include "fxerror.h"

int main() {
        fx_pool pool;
        fx_error err = fxpool_create(100000, B, 2, 8, &pool);
        if(err == FX_RES_OK) {
                printf("Memory allocated\n");
        }
        return 0;
}

