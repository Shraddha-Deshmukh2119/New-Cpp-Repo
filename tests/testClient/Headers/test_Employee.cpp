#include <gtest/gtest.h>

#include "Employee.h"
#include "MockClient.h"
#include "stdin_redirect.h"

class TestableClientEmployee : public Employee
{
public:
    explicit TestableClientEmployee(MockClient& client) : Employee(client) {}

    void invokeHomeExit() { home(); }
    void invokeComplainExit() { complain(); }
};

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
