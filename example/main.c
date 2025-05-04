#include <stdio.h>
#include <fxpool.h>
#include <fxerror.h>

void bug() {
        printf("bug point\n");
}

int main() {
        fx_pool pool = {0};

        fx_error err = fxpool_create_large_pool(4, KB, 27648, &pool, (FXPOOL_SIZE_AUTO_GROW | FXPOOL_SIZE_CUSTOM));
        fxpool_log(&pool);
        printf("%lu\n", fxpool_capacity(&pool));

        int* ptr;
        ptr = (int*)fxpool_alloc(&pool);

        *ptr = 20;

        printf("value : %d\n", *ptr);

        fxpool_dealloc(ptr, &pool);
        fxpool_log(&pool);
        fxpool_destroy_large_pool(&pool);
        return 0;
}
