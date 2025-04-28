extern "C" {
#include "../fxpool.h"
#include "../fxerror.h"
#include "../fxsys.h"
}

#include <gtest/gtest.h>

TEST(fxpool_core, create) 
{
        fx_pool pool;
        EXPECT_EQ(fxpool_create(100,  KB, 10,  ALIGN_16, &pool), FX_RES_OK);
        EXPECT_EQ(fxpool_create(100,  KB, 10,  27, &pool), FX_RES_ALIGNED);
        EXPECT_EQ(fxpool_create(100,  KB, 1000000000000000000,  ALIGN_16, &pool), FX_RES_MEMORY);
        EXPECT_EQ(fxpool_create(-300, KB, 10,  ALIGN_4,  &pool), FX_RES_PARAM);
        EXPECT_EQ(fxpool_create(10,   MB, 20,  ALIGN_8,  NULL),  FX_RES_PARAM);
}

TEST(fxpool_core, alloc)
{
        void* ptr = NULL;
        fx_pool pool;
        fxpool_create(10, KB, 200, ALIGN_16, &pool);
        EXPECT_EQ(fxpool_alloc(NULL), ptr);
        EXPECT_NE(fxpool_alloc(&pool), ptr);
}

TEST(fxpool_core, destroy) 
{
        fx_pool pool;
        fxpool_create(10, KB, 200, ALIGN_8, &pool);
        EXPECT_EQ(fxpool_destroy(NULL), FX_RES_PARAM);
        EXPECT_EQ(fxpool_destroy(&pool), FX_RES_OK);
}

TEST(fxpool_core, dealloc) 
{
        fx_pool pool;
        fxpool_create(10, KB, 200, ALIGN_8, &pool);
        void* ptr = fxpool_alloc(&pool);
        EXPECT_EQ(fxpool_dealloc(NULL, NULL), FX_RES_PARAM);
        EXPECT_EQ(fxpool_dealloc(ptr, &pool), FX_RES_OK);
}
