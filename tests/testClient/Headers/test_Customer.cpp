#include <gtest/gtest.h>

#include "MockClient.h"
#include "stdin_redirect.h"
#include "test_Customer.h"

TEST(ClientCustomerTest, CanConstruct)
{
    MockClient mock;
    Customer customer(mock);
    SUCCEED();
}

TEST(ClientCustomerTest, StartBackSendsChoice)
{
    MockClient mock;
    TestableClientCustomer customer(mock);
    StdinRedirect input("3\n");
    customer.invokeStartBack();
    ASSERT_FALSE(mock.sent().empty());
    EXPECT_EQ(mock.sent().back(), "3");
}

TEST(ClientCustomerTest, HomeLogoutSendsChoice)
{
    MockClient mock;
    TestableClientCustomer customer(mock);
    StdinRedirect input("4\n");
    customer.invokeHomeLogout();
    ASSERT_FALSE(mock.sent().empty());
    EXPECT_EQ(mock.sent().back(), "4");
}
