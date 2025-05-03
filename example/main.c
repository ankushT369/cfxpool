#include <stdio.h>
#include <fxpool.h>
#include <fxerror.h>

void bug() {
        printf("bug point\n");
}

int main() {
        fx_pool pool = {0};

        fx_error err = fxpool_create_large_pool(4, KB, 883, &pool, (FXPOOL_SIZE_AUTO_GROW | FXPOOL_SIZE_CUSTOM));
        fxpool_log(&pool);
        printf("%lu\n", fxpool_capacity(&pool));

        int* ptr;
        ptr = (int*)fxpool_alloc(&pool);

        int a = 20;
        ptr = &a;

        printf("value : %d\n", *ptr);

        fxpool_dealloc(ptr, &pool);
        //fxpool_destroy(&pool);
        return 0;
}
