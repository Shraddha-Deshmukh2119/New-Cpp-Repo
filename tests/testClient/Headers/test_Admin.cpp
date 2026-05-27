#include <gtest/gtest.h>

#include <algorithm>
#include <ctime>
#include <string>

#include "MockClient.h"
#include "stdin_redirect.h"
#include "test_Admin.h"
#include "test_helpers.h"

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
    admin_.invokeHome();
    ASSERT_FALSE(mock_.sent().empty());
    EXPECT_EQ(mock_.sent().back(), "17");
}

TEST_F(ClientAdminTest, HomeInvalidChoiceThenExit)
{
    StdinRedirect input("99\n17\n");
    admin_.invokeHome();
    EXPECT_EQ(mock_.sent().back(), "17");
}

TEST_F(ClientAdminTest, HomeViewCustomersSendsChoice)
{
    mock_.enqueue("Customer list\n");
    StdinRedirect input("5\n17\n");
    admin_.invokeHome();
    EXPECT_TRUE(std::find(mock_.sent().begin(), mock_.sent().end(), "5") != mock_.sent().end());
}

TEST_F(ClientAdminTest, HomeActivityReceivesPayload)
{
    mock_.enqueue("Activity log line\n");
    StdinRedirect input("15\n17\n");
    admin_.invokeHome();
    SUCCEED();
}

TEST_F(ClientAdminTest, AddEmployeeSuccess)
{
    StdinRedirect input(test_helpers::clientPersonInputStdin("emp1", "Emp One", 28));
    mock_.enqueue("TRUE");
    admin_.invokeAddEmployee();
    SUCCEED();
}

TEST_F(ClientAdminTest, AddEmployeeFailure)
{
    StdinRedirect input(test_helpers::clientPersonInputStdin("emp2", "Emp Two", 28, "e2@example.com"));
    mock_.enqueue("FALSE");
    admin_.invokeAddEmployee();
    SUCCEED();
}

TEST_F(ClientAdminTest, BalanceNegativeRetryThenSuccess)
{
    StdinRedirect input("user1\n-5\n25\n");
    mock_.enqueue("TRUE");
    admin_.invokeBalance();
    SUCCEED();
}

TEST_F(ClientAdminTest, BalanceUpdateFailure)
{
    StdinRedirect input("user1\n50\n");
    mock_.enqueue("FALSE");
    admin_.invokeBalance();
    SUCCEED();
}

TEST_F(ClientAdminTest, StockOrderUnknownGood)
{
    mock_.enqueue("Stock report\n");
    StdinRedirect input("1\nUnknown\n3\n");
    mock_.enqueue("FALSE");
    admin_.invokeStock();
    SUCCEED();
}

TEST_F(ClientAdminTest, StockOrderSuccess)
{
    mock_.enqueue("Stock report\n");
    StdinRedirect input("1\nItem0\n5\n");
    mock_.enqueue("TRUE");
    mock_.enqueue("TRUE");
    admin_.invokeStock();
    SUCCEED();
}

TEST_F(ClientAdminTest, StockOrderInsufficientCash)
{
    mock_.enqueue("Stock report\n");
    StdinRedirect input("1\nItem0\n5\n");
    mock_.enqueue("TRUE");
    mock_.enqueue("FALSE");
    admin_.invokeStock();
    SUCCEED();
}

TEST_F(ClientAdminTest, StockModifySuccess)
{
    mock_.enqueue("Stock report\n");
    StdinRedirect input("2\nItem0\n12.5\n3\n4\n3\n");
    mock_.enqueue("TRUE");
    admin_.invokeStock();
    SUCCEED();
}

TEST_F(ClientAdminTest, StockModifyUnknownGood)
{
    mock_.enqueue("Stock report\n");
    StdinRedirect input("2\nMissing\n3\n");
    mock_.enqueue("FALSE");
    admin_.invokeStock();
    SUCCEED();
}
