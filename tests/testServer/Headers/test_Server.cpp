#include <gtest/gtest.h>
#include "Server.h"
#include "test_helpers.h"

static void noopHandler(Thread&)
{
}

TEST(ServerNetworkingTest, StartInitializesWinsock)
{
    Server server(noopHandler, test_helpers::testPort());
    EXPECT_TRUE(server.start());
}
