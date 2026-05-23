#include <gtest/gtest.h>
#include "Cash.h"

TEST(ClientCashTest, OperatorsTrackInAndOut)
{
    Cash cash;
    cash + 50.0;
    cash - 20.0;
    EXPECT_DOUBLE_EQ(0.0, cash.get_final_cash());
}

TEST(ClientCashTest, InitialFinalCashIsZero)
{
    Cash cash;
    EXPECT_DOUBLE_EQ(0.0, cash.get_final_cash());
}
