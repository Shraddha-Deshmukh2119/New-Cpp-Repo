#include <gtest/gtest.h>

#include "MockClient.h"
#include "stdin_redirect.h"
#include "test_Employee.h"

TEST(ClientEmployeeTest, CanConstruct)
{
    MockClient mock;
    Employee employee(mock);
    SUCCEED();
}

TEST(ClientEmployeeTest, HomeLogoutSendsChoice)
{
    MockClient mock;
    TestableClientEmployee employee(mock);
    StdinRedirect input("4\n");
    employee.invokeHomeExit();
    ASSERT_FALSE(mock.sent().empty());
    EXPECT_EQ(mock.sent().back(), "4");
}

TEST(ClientEmployeeTest, ComplainMenuExit)
{
    MockClient mock;
    TestableClientEmployee employee(mock);
    StdinRedirect input("7\n");
    employee.invokeComplainExit();
    EXPECT_FALSE(mock.sent().empty());
}
