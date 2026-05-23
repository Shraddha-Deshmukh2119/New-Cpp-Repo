#include <gtest/gtest.h>
#include "Cash.h"

TEST(ServerCashTest, CashInUpdatesFinalBalance)
{
    Cash cash;
    cash + 100.0;
    EXPECT_DOUBLE_EQ(100.0, cash.get_final_cash());
}

TEST(ServerCashTest, CashOutReducesFinalBalance)
{
    Cash cash;
    cash + 100.0;
    cash - 30.0;
    EXPECT_DOUBLE_EQ(70.0, cash.get_final_cash());
}
