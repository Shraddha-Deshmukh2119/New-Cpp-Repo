#include <gtest/gtest.h>
#include "Client.h"
#include "test_helpers.h"

TEST(ClientNetworkingTest, StartInitializesWinsock)
{
    Client client("127.0.0.1", test_helpers::testPort());
    EXPECT_TRUE(client.start());
}

TEST(ClientNetworkingTest, SendWithoutServerDoesNotCrash)
{
    Client client("127.0.0.1", test_helpers::testPort());
    ASSERT_TRUE(client.start());
    const int sent = client.Send("ping");
    (void)sent;
    SUCCEED();
}
