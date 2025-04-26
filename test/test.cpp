extern "C" {
#include "../fxpool.h"
#include "../fxerror.h"
#include "../fxsys.h"
}

#include <gtest/gtest.h>

TEST(mem, pool) {
        fx_pool pool;
        EXPECT_EQ(fxpool_create(100, KB, 10, ALIGN_16, &pool), FX_RES_OK);
        EXPECT_EQ(fxpool_create(100, TB, 10, ALIGN_16, &pool), FX_RES_MEMORY);
        EXPECT_EQ(fxpool_create(0, TB, 10, ALIGN_16, &pool), FX_RES_PARAM);
        EXPECT_EQ(fxpool_create(3, B, 0, ALIGN_16, &pool), FX_RES_PARAM);
        EXPECT_EQ(fxpool_create(3, B, 0, 37, &pool), FX_RES_ALIGNED);
        EXPECT_EQ(fxpool_create(3, B, 0, -8, &pool), FX_RES_ALIGNED);
        EXPECT_EQ(fxpool_create(3, B, 10, -248, &pool), FX_RES_OK);
        EXPECT_EQ(fxpool_create(-300, B, 10, -248, &pool), FX_RES_PARAM);
}
