#include <gtest/gtest.h>

#include <ctime>

#include "stdin_redirect.h"
#include "test_Customer.h"
#include "test_helpers.h"

class ClientCustomerFlowTest : public ::testing::Test
{
protected:
    MockClient mock_;
    TestableClientCustomer customer_{mock_};
};

TEST_F(ClientCustomerFlowTest, StartSignupPath)
{
    StdinRedirect input("1\n" + test_helpers::clientPersonInputStdin("cust1", "Cust One", 25, "c@example.com", "0300333") +
                        "3\n");
    mock_.enqueue("true");
    customer_.invokeStart();
    SUCCEED();
}

TEST_F(ClientCustomerFlowTest, StartInvalidThenBack)
{
    StdinRedirect input("9\n3\n");
    customer_.invokeStart();
    EXPECT_EQ(mock_.sent().back(), "3");
}

TEST_F(ClientCustomerFlowTest, HomeBuyAndLogout)
{
    const std::string menu = "   1 Item0        10\n\nYour balance: $100\n";
    mock_.enqueue(menu);
    mock_.enqueue("NO-ORDER");
    mock_.enqueue(menu);
    StdinRedirect input("1\n2\n3\n4\n");
    customer_.invokeHome();
    EXPECT_EQ(mock_.sent().back(), "4");
}

TEST_F(ClientCustomerFlowTest, HomeComplainThenLogout)
{
    StdinRedirect input("2\n4\n4\n");
    customer_.invokeHome();
    EXPECT_EQ(mock_.sent().back(), "4");
}

TEST_F(ClientCustomerFlowTest, ComplainWriteSuccess)
{
    StdinRedirect input("1\nMy complaint text\n4\n");
    mock_.enqueue("TRUE");
    customer_.invokeComplain();
    EXPECT_FALSE(mock_.sent().empty());
}

TEST_F(ClientCustomerFlowTest, ComplainWriteDuplicate)
{
    StdinRedirect input("1\nAnother complaint\n4\n");
    mock_.enqueue("FALSE");
    customer_.invokeComplain();
    SUCCEED();
}

TEST_F(ClientCustomerFlowTest, ComplainReview)
{
    StdinRedirect input("2\n4\n");
    mock_.enqueue("Existing complaint body\n");
    customer_.invokeComplain();
    SUCCEED();
}

TEST_F(ClientCustomerFlowTest, ComplainChangeSuccess)
{
    StdinRedirect input("3\nUpdated text\n4\n");
    mock_.enqueue("TRUE");
    customer_.invokeComplain();
    SUCCEED();
}

TEST_F(ClientCustomerFlowTest, ComplainChangeFailedAnswered)
{
    StdinRedirect input("3\nUpdated text\n4\n");
    mock_.enqueue("FAILED");
    customer_.invokeComplain();
    SUCCEED();
}

TEST_F(ClientCustomerFlowTest, ComplainChangeNotFound)
{
    StdinRedirect input("3\nUpdated text\n4\n");
    mock_.enqueue("OTHER");
    customer_.invokeComplain();
    SUCCEED();
}

TEST_F(ClientCustomerFlowTest, ComplainInvalidChoice)
{
    StdinRedirect input("9\n4\n");
    customer_.invokeComplain();
    SUCCEED();
}
