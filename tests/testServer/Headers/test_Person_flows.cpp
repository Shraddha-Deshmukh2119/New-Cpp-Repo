#include <gtest/gtest.h>

#include <fstream>

#include "test_Person.h"
#include "test_workspace.h"

class ServerPersonFlowTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        test_workspace::create("oss_server_person_flows");
        mock_ = std::make_unique<MockThread>();
        person_ = std::make_unique<TestableServerPerson>(*mock_);
    }

    std::unique_ptr<MockThread> mock_;
    std::unique_ptr<TestableServerPerson> person_;
};

TEST_F(ServerPersonFlowTest, InputWritesNewRecord)
{
    const std::string file = "input_new.txt";
    mock_->enqueue("Server User");
    mock_->enqueue("28");
    mock_->enqueue("F");
    mock_->enqueue("3");
    mock_->enqueue("10");
    mock_->enqueue("1998");
    mock_->enqueue("12345-1234567-2");
    mock_->enqueue("server@example.com");
    mock_->enqueue("03009998877");
    mock_->enqueue("srvuser");
    mock_->enqueue("pass");

    EXPECT_TRUE(person_->testInput(file));

    std::ifstream in(file);
    std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    EXPECT_NE(contents.find("srvuser"), std::string::npos);
}

TEST_F(ServerPersonFlowTest, LoginSuccess)
{
    const std::string file = "login_ok.txt";
    test_workspace::writeFile(file, test_workspace::sampleUserRecord("loginok", "secret"));

    mock_->enqueue("loginok");
    mock_->enqueue("secret");
    person_->invokeLoginOnce(file);
    EXPECT_EQ(mock_->sent().back(), "Correct");
}

TEST_F(ServerPersonFlowTest, LoginWrongUser)
{
    const std::string file = "login_miss.txt";
    test_workspace::writeFile(file, test_workspace::sampleUserRecord("exists", "secret"));

    mock_->enqueue("ghost");
    mock_->enqueue("secret");
    person_->invokeLoginOnce(file);
    EXPECT_EQ(mock_->sent().back(), "Incorrect-User");
}

TEST_F(ServerPersonFlowTest, LoginWrongPassword)
{
    const std::string file = "login_pass.txt";
    test_workspace::writeFile(file, test_workspace::sampleUserRecord("exists2", "secret"));

    mock_->enqueue("exists2");
    mock_->enqueue("wrong");
    person_->invokeLoginOnce(file);
    EXPECT_EQ(mock_->sent().back(), "Incorrect-Pass");
}

TEST_F(ServerPersonFlowTest, BuyAddAndConfirmOrder)
{
    test_workspace::writeSampleGoodsFile();
    test_workspace::writeSampleCashFile();
    const std::string file = "buy_user.txt";
    test_workspace::writeFile(file, test_workspace::sampleUserRecord("buyer", "secret", "500"));
    person_->setLookup("Username: buyer");

    mock_->enqueue("1");
    mock_->enqueue("1");
    mock_->enqueue("2");
    mock_->enqueue("2");
    mock_->enqueue("3");

    person_->invokeBuy(file);
    EXPECT_FALSE(mock_->sent().empty());
}

TEST_F(ServerPersonFlowTest, BuyNotEnoughStock)
{
    test_workspace::writeSampleGoodsFile();
    test_workspace::writeSampleCashFile();
    const std::string file = "buy_low_stock.txt";
    test_workspace::writeFile(file, test_workspace::sampleUserRecord("buyer2", "secret", "500"));
    person_->setLookup("Username: buyer2");

    mock_->enqueue("1");
    mock_->enqueue("1");
    mock_->enqueue("999");
    mock_->enqueue("3");

    person_->invokeBuy(file);
    EXPECT_EQ(mock_->sent().back(), "NOT-ENOUGH");
}

TEST_F(ServerPersonFlowTest, UpdateBalancePersists)
{
    const std::string file = "upd_bal.txt";
    test_workspace::writeFile(file, test_workspace::sampleUserRecord("baluser", "secret", "100"));
    person_->setLookup("Username: baluser");
    EXPECT_TRUE(person_->testUpdateBalance(file));

    std::ifstream in(file);
    std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    EXPECT_NE(contents.find("Balance: 150"), std::string::npos);
}

TEST_F(ServerPersonFlowTest, UpdateGoodsAndCash)
{
    test_workspace::writeSampleGoodsFile();
    test_workspace::writeSampleCashFile();
    EXPECT_TRUE(person_->testUpdateGoods());
    EXPECT_TRUE(person_->testUpdateCash());
}
