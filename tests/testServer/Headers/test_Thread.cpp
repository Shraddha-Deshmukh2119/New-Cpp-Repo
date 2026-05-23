#include <gtest/gtest.h>
#include "Server.h"
#include "Thread.h"

static void noopHandler(Thread&)
{
}

TEST(ServerThreadTest, DefaultThreadIsAvailable)
{
    Thread thread;
    EXPECT_TRUE(thread.available());
}
