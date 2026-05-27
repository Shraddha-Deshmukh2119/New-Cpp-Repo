#include <gtest/gtest.h>

#include <ctime>
#include <string>

#include "MockClient.h"
#include "stdin_redirect.h"
#include "test_helpers.h"
#include "test_Person.h"

class ClientPersonFlowTest : public ::testing::Test
{
protected:
    MockClient mock_;
    TestablePerson person_{mock_};
};

TEST_F(ClientPersonFlowTest, DobRejectsInvalidYear)
{
    Date dob{1, 1, 1900};
    EXPECT_FALSE(person_.testCheckDOB(dob));
}

TEST_F(ClientPersonFlowTest, DobRejectsInvalidDayForApril)
{
    Date dob{31, 4, 2000};
    EXPECT_FALSE(person_.testCheckDOB(dob));
}

TEST_F(ClientPersonFlowTest, DobAcceptsFebruaryDay28)
{
    Date dob{28, 2, 2000};
    EXPECT_TRUE(person_.testCheckDOB(dob));
}

TEST_F(ClientPersonFlowTest, DobAccepts31DayMonth)
{
    Date dob{31, 1, 2000};
    EXPECT_TRUE(person_.testCheckDOB(dob));
}

TEST_F(ClientPersonFlowTest, CnicRejectsWrongDashPosition)
{
    EXPECT_FALSE(person_.testCheckCNIC("1234-1234567-12345"));
}

TEST_F(ClientPersonFlowTest, ConsistencyMatchesAge)
{
    const std::string dobStr = test_helpers::dobStringForAge(25);
    const int year = std::stoi(dobStr.substr(dobStr.find_last_of('/') + 1));
    Date dob{1, 1, year};
    EXPECT_TRUE(person_.testConsistency(dob, 25));
}

TEST_F(ClientPersonFlowTest, ConsistencyRejectsMismatchedAge)
{
    Date dob{15, 6, 2000};
    EXPECT_FALSE(person_.testConsistency(dob, 50));
}

TEST_F(ClientPersonFlowTest, InputSuccessPath)
{
    StdinRedirect input(test_helpers::clientPersonInputStdin(
        "flowuser", "Flow User", 30, "flow@example.com", "03005551212"));
    mock_.enqueue("true");
    EXPECT_TRUE(person_.invokeInput());
}

TEST_F(ClientPersonFlowTest, InputFailurePath)
{
    StdinRedirect input(test_helpers::clientPersonInputStdin(
        "flowuser2", "Flow User", 30, "flow2@example.com", "03005551213"));
    mock_.enqueue("false");
    EXPECT_FALSE(person_.invokeInput());
}

TEST_F(ClientPersonFlowTest, LoginSuccessEntersHome)
{
    StdinRedirect input("gooduser\n");
    mock_.enqueue("Correct");
    person_.invokeLogin();
    SUCCEED();
}

TEST_F(ClientPersonFlowTest, LoginWrongUserMessage)
{
    StdinRedirect input("missing\n");
    mock_.enqueue("Incorrect-User");
    person_.invokeLoginOnce();
    SUCCEED();
}

TEST_F(ClientPersonFlowTest, LoginWrongPasswordMessage)
{
    StdinRedirect input("userx\n");
    mock_.enqueue("Incorrect-Pass");
    person_.invokeLoginOnce();
    SUCCEED();
}

TEST_F(ClientPersonFlowTest, ProfileDisplaysRecord)
{
    mock_.enqueue(" --> Username: demo\n --> Name: Demo User\n\n");
    person_.invokeProfile();
    SUCCEED();
}

TEST_F(ClientPersonFlowTest, BuyAddItemNotEnoughStock)
{
    const std::string menu =
        "   1 Item0        10\n\nYour balance: $100\n";
    mock_.enqueue(menu);
    mock_.enqueue("NOT-ENOUGH");
    mock_.enqueue(menu);
    StdinRedirect input("1\n1\n5\n3\n");
    person_.invokeBuy();
    EXPECT_FALSE(mock_.sent().empty());
}

TEST_F(ClientPersonFlowTest, BuyConfirmNoOrder)
{
    const std::string menu =
        "   1 Item0        10\n\nYour balance: $100\n";
    mock_.enqueue(menu);
    mock_.enqueue("NO-ORDER");
    mock_.enqueue(menu);
    StdinRedirect input("2\n3\n");
    person_.invokeBuy();
    SUCCEED();
}

TEST_F(ClientPersonFlowTest, BuyInsufficientBalanceOnConfirm)
{
    const std::string menu =
        "   1 Item0        10\n\nYour balance: $5\n";
    mock_.enqueue(menu);
    mock_.enqueue("Your balance: $5\nCost of the order: $50\n");
    mock_.enqueue(menu);
    StdinRedirect input("2\n3\n");
    person_.invokeBuy();
    SUCCEED();
}

TEST_F(ClientPersonFlowTest, BuyPlaceOrderSuccess)
{
    const std::string menu =
        "   1 Item0        10\n\nYour balance: $100\n";
    mock_.enqueue(menu);
    mock_.enqueue("Cost of the order: $20\nYour new balance: $80\n");
    mock_.enqueue(menu);
    StdinRedirect input("2\n3\n");
    person_.invokeBuy();
    SUCCEED();
}

TEST_F(ClientPersonFlowTest, BuyInvalidMenuChoice)
{
    const std::string menu =
        "   1 Item0        10\n\nYour balance: $100\n";
    mock_.enqueue(menu);
    mock_.enqueue(menu);
    StdinRedirect input("9\n3\n");
    person_.invokeBuy();
    SUCCEED();
}
