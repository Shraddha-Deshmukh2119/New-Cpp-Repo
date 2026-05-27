#include <gtest/gtest.h>

#include "stdin_redirect.h"
#include "test_Employee.h"

class ClientEmployeeFlowTest : public ::testing::Test
{
protected:
    MockClient mock_;
    TestableClientEmployee employee_{mock_};
};

TEST_F(ClientEmployeeFlowTest, HomeBuyProfileLogout)
{
    const std::string menu = "   1 Item0        10\n\nYour balance: $100\n";
    mock_.enqueue(menu);
    mock_.enqueue("NO-ORDER");
    mock_.enqueue(menu);
    mock_.enqueue("profile data\n");
    StdinRedirect input("1\n2\n3\n3\n4\n");
    employee_.invokeHome();
    EXPECT_EQ(mock_.sent().back(), "4");
}

TEST_F(ClientEmployeeFlowTest, HomeInvalidChoice)
{
    StdinRedirect input("9\n4\n");
    employee_.invokeHome();
    EXPECT_EQ(mock_.sent().back(), "4");
}

TEST_F(ClientEmployeeFlowTest, ComplainSeeNamedSuccess)
{
    StdinRedirect input("1\nAlice\n");
    mock_.enqueue("TRUE");
    mock_.enqueue("Complaint details\n");
    mock_.enqueue("7\n");
    employee_.invokeComplain();
    SUCCEED();
}

TEST_F(ClientEmployeeFlowTest, ComplainSeeNamedMissing)
{
    StdinRedirect input("1\nNobody\n7\n");
    mock_.enqueue("FALSE");
    employee_.invokeComplain();
    SUCCEED();
}

TEST_F(ClientEmployeeFlowTest, ComplainSeeAnonymous)
{
    StdinRedirect input("2\nBob\n7\n");
    mock_.enqueue("TRUE");
    mock_.enqueue("Anonymous complaint\n");
    employee_.invokeComplain();
    SUCCEED();
}

TEST_F(ClientEmployeeFlowTest, ComplainAnswerSuccess)
{
    StdinRedirect input("3\nCarol\nWe are sorry\n7\n");
    mock_.enqueue("TRUE");
    employee_.invokeComplain();
    SUCCEED();
}

TEST_F(ClientEmployeeFlowTest, ComplainAnswerMissing)
{
    StdinRedirect input("3\nDave\nResponse\n7\n");
    mock_.enqueue("FALSE");
    employee_.invokeComplain();
    SUCCEED();
}

TEST_F(ClientEmployeeFlowTest, ComplainViewAllComplaintees)
{
    StdinRedirect input("4\n7\n");
    mock_.enqueue("Alice, Bob\n");
    employee_.invokeComplain();
    SUCCEED();
}

TEST_F(ClientEmployeeFlowTest, ComplainViewAllComplaints)
{
    StdinRedirect input("5\n7\n");
    mock_.enqueue("All complaints list\n");
    employee_.invokeComplain();
    SUCCEED();
}

TEST_F(ClientEmployeeFlowTest, ComplainClearAll)
{
    StdinRedirect input("6\n7\n");
    employee_.invokeComplain();
    SUCCEED();
}

TEST_F(ClientEmployeeFlowTest, ComplainInvalidChoice)
{
    StdinRedirect input("9\n7\n");
    employee_.invokeComplain();
    SUCCEED();
}
