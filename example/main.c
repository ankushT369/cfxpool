#include <stdio.h>
#include <fxpool.h>
#include <fxerror.h>

typedef struct student {
        int arr[4096];
} student;

void bug() {
        printf("bug point\n");
}

int main() {
        fx_pool pool = {0};

        fx_error err = fxpool_create_large_pool(sizeof(student), B, 276400, &pool, (FXPOOL_SIZE_AUTO_GROW | FXPOOL_SIZE_CUSTOM));
        //fxpool_log(&pool);
        printf("%lu\n", fxpool_capacity(&pool));

        student* ptr;
        for(int iter = 0; iter < 200000; iter++) {
                ptr = (student*)fxpool_alloc(&pool);


                for(int i = 0; i < 4096; i++) {
                        ptr->arr[i] = i + 100;
                }
        }

        /*
        for(int iter = 0; iter < 10000; iter++) {
                fxpool_dealloc(ptr, &pool);
                ptr = (student*)fxpool_alloc(&pool);

                for(int i = 0; i < 4096; i++) {
                        ptr->arr[i] = i + 100;
                }
        }
        */

        //fxpool_dealloc(ptr, &pool);
        //fxpool_log(&pool);
        fxpool_destroy_large_pool(&pool);
        return 0;
}
