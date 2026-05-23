#include <gtest/gtest.h>

#include <string>

#include "MockClient.h"
#include "stdin_redirect.h"
#include "test_Admin.h"

class ClientAdminTest : public ::testing::Test
{
protected:
    MockClient mock_;
    TestableClientAdmin admin_{mock_};
};

TEST_F(ClientAdminTest, CanConstruct)
{
    SUCCEED();
}

TEST_F(ClientAdminTest, ViewReceivesAndDisplaysPayload)
{
    mock_.enqueue("Customer listing line 1\nCustomer listing line 2\n");
    admin_.invokeView("CUSTOMERS");
    SUCCEED();
}

TEST_F(ClientAdminTest, SearchKeyFoundShowsProfilePath)
{
    StdinRedirect input("\n");
    mock_.enqueue("TRUE");
    mock_.enqueue(" --> Username: found_user\n");
    admin_.invokeSearchKey("CNIC");
    SUCCEED();
}

TEST_F(ClientAdminTest, SearchKeyNotFound)
{
    StdinRedirect input("\n");
    mock_.enqueue("FALSE");
    admin_.invokeSearchKey("Email");
    SUCCEED();
}

TEST_F(ClientAdminTest, DeleteSendsUsername)
{
    StdinRedirect input("user_to_delete\n");
    admin_.invokeDelete();
    ASSERT_FALSE(mock_.sent().empty());
    EXPECT_EQ(mock_.sent().front(), "user_to_delete");
}

TEST_F(ClientAdminTest, BalanceSendsUserAndAmount)
{
    StdinRedirect input("balance_user\n50\n");
    mock_.enqueue("TRUE");
    admin_.invokeBalance();
    ASSERT_GE(mock_.sent().size(), 2u);
    EXPECT_EQ(mock_.sent()[0], "balance_user");
}

TEST_F(ClientAdminTest, StockReceivesInventory)
{
    mock_.enqueue("Stock line A\nStock line B\n");
    StdinRedirect input("3\n");
    admin_.invokeStock();
    SUCCEED();
}

TEST_F(ClientAdminTest, AccountsReceivesCashReport)
{
    mock_.enqueue("Initial: 100\nFinal: 200\n");
    admin_.invokeAccounts();
    SUCCEED();
}

TEST_F(ClientAdminTest, HomeExitSendsBackChoice)
{
    StdinRedirect input("17\n");
    admin_.invokeHomeExit();
    ASSERT_FALSE(mock_.sent().empty());
    EXPECT_EQ(mock_.sent().back(), "17");
}
