#include <stdio.h>
#include <fxpool.h>

int main() {
        fx_pool pool;

        fxpool_create(4, B, 883, ALIGN_8, &pool);

        int* ptr;
        ptr = (int*)fxpool_alloc(&pool);

        int a = 20;

        ptr = &a;
        
        printf("value %d\n", *ptr);

        fxpool_destroy(&pool);
        return 0;
}
