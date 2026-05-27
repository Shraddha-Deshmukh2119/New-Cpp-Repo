#include <gtest/gtest.h>

#include <algorithm>
#include <fstream>
#include <string>

#include "MockThread.h"
#include "test_Admin.h"
#include "test_helpers.h"
#include "test_workspace.h"

class ServerAdminTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        workDir_ = test_workspace::create("oss_server_admin");
        mock_ = std::make_unique<MockThread>();
        admin_ = std::make_unique<TestableServerAdmin>(*mock_);
    }

    std::filesystem::path workDir_;
    std::unique_ptr<MockThread> mock_;
    std::unique_ptr<TestableServerAdmin> admin_;
};

TEST_F(ServerAdminTest, ViewSendsFileContents)
{
    const std::string file = test_helpers::tempFile("customers_view.txt");
    test_workspace::writeFile(file, test_workspace::sampleUserRecord("alice"));

    admin_->invokeView(file);
    ASSERT_FALSE(mock_->sent().empty());
    EXPECT_NE(mock_->sent().back().find("alice"), std::string::npos);
}

TEST_F(ServerAdminTest, ViewHandlesMissingFile)
{
    admin_->invokeView("missing_users.txt");
    SUCCEED();
}

TEST_F(ServerAdminTest, SearchFindsUserByCnic)
{
    const std::string file = test_helpers::tempFile("emp_search.txt");
    test_workspace::writeFile(file, test_workspace::sampleUserRecord("worker1"));

    mock_->enqueue("12345-1234567-1");
    admin_->invokeSearchKey("CNIC", file);

    EXPECT_TRUE(std::any_of(mock_->sent().begin(), mock_->sent().end(),
                            [](const std::string& s) { return s == "TRUE"; }));
}

TEST_F(ServerAdminTest, SearchReturnsFalseWhenNotFound)
{
    const std::string file = test_helpers::tempFile("emp_search_miss.txt");
    test_workspace::writeFile(file, test_workspace::sampleUserRecord("worker2"));

    mock_->enqueue("00000-0000000-0");
    admin_->invokeSearchKey("CNIC", file);
    EXPECT_EQ(mock_->sent().back(), "FALSE");
}

TEST_F(ServerAdminTest, DeleteRemovesUserBlock)
{
    const std::string file = "emp_delete.txt";
    test_workspace::writeFile(file,
                              test_workspace::sampleUserRecord("keep") +
                                  test_workspace::sampleUserRecord("remove"));

    mock_->enqueue("remove");
    admin_->invokeDelete(file);

    std::ifstream in(file);
    std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    EXPECT_NE(contents.find("keep"), std::string::npos);
    EXPECT_EQ(contents.find("remove"), std::string::npos);
}

TEST_F(ServerAdminTest, BalanceUpdatesRecord)
{
    const std::string file = "cust_balance.txt";
    test_workspace::writeFile(file, test_workspace::sampleUserRecord("payer", "secret", "50"));

    mock_->enqueue("payer");
    mock_->enqueue("200");
    admin_->invokeBalance(file);
    EXPECT_EQ(mock_->sent().back(), "TRUE");

    std::ifstream in(file);
    std::string line;
    bool found = false;
    while (std::getline(in, line))
    {
        if (line == "Balance: 200")
            found = true;
    }
    EXPECT_TRUE(found);
}

TEST_F(ServerAdminTest, AccountsReadsCashFile)
{
    test_workspace::writeSampleCashFile();
    admin_->invokeAccounts();
    ASSERT_FALSE(mock_->sent().empty());
    EXPECT_NE(mock_->sent().back().find("Final"), std::string::npos);
}

TEST_F(ServerAdminTest, StockModifyPath)
{
    test_workspace::writeSampleGoodsFile();
    test_workspace::writeSampleCashFile();

    mock_->enqueue("2");
    mock_->enqueue("Item0");
    mock_->enqueue("15.5");
    mock_->enqueue("4");
    mock_->enqueue("6");

    admin_->invokeStock();
    EXPECT_FALSE(mock_->sent().empty());
}

TEST_F(ServerAdminTest, HomeMenuPartialCoverage)
{
    test_workspace::writeFile("customer.txt", test_workspace::sampleUserRecord("home_user"));

    mock_->enqueue("5");
    mock_->enqueue("17");
    admin_->invokeHome();

    EXPECT_TRUE(std::any_of(mock_->sent().begin(), mock_->sent().end(),
                            [](const std::string& s) { return s.find("home_user") != std::string::npos; }));
}

TEST_F(ServerAdminTest, HomeBuyAndProfilePaths)
{
    test_workspace::writeSampleGoodsFile();
    test_workspace::writeSampleCashFile();
    test_workspace::writeFile("admin.txt", test_workspace::sampleUserRecord("admin_buyer", "secret", "500"));
    admin_->setLookup("Username: admin_buyer");

    mock_->enqueue("1");
    mock_->enqueue("1");
    mock_->enqueue("1");
    mock_->enqueue("2");
    mock_->enqueue("3");
    mock_->enqueue("3");
    mock_->enqueue("17");
    admin_->invokeHome();
    EXPECT_FALSE(mock_->sent().empty());
}

TEST_F(ServerAdminTest, HomeSearchEmployees)
{
    test_workspace::writeFile("emp.txt", test_workspace::sampleUserRecord("emp_search"));
    mock_->enqueue("9");
    mock_->enqueue("1");
    mock_->enqueue("12345-1234567-1");
    mock_->enqueue("17");
    admin_->invokeHome();
    SUCCEED();
}

TEST_F(ServerAdminTest, HomeActivityPath)
{
    mock_->enqueue("15");
    mock_->enqueue("17");
    admin_->invokeHome();
    EXPECT_FALSE(mock_->sent().empty());
}

TEST_F(ServerAdminTest, AddEmployeeSuccess)
{
    test_workspace::writeFile("emp.txt", "");
    mock_->enqueue("New Emp");
    mock_->enqueue("30");
    mock_->enqueue("M");
    mock_->enqueue("1");
    mock_->enqueue("15");
    mock_->enqueue("1995");
    mock_->enqueue("12345-1234567-1");
    mock_->enqueue("new@example.com");
    mock_->enqueue("03001112233");
    mock_->enqueue("newemp");
    mock_->enqueue("pass");
    admin_->invokeAddEmployee();
    EXPECT_EQ(mock_->sent().back(), "TRUE");
}

TEST_F(ServerAdminTest, SearchMenuPhonePath)
{
    const std::string file = test_helpers::tempFile("search_phone.txt");
    test_workspace::writeFile(file, test_workspace::sampleUserRecord("phone_user"));
    mock_->enqueue("3");
    mock_->enqueue("03001234567");
    mock_->enqueue("6");
    admin_->invokeSearchMenu(file);
    SUCCEED();
}

TEST_F(ServerAdminTest, StockOrderSuccess)
{
    test_workspace::writeSampleGoodsFile();
    test_workspace::writeSampleCashFile();
    mock_->enqueue("1");
    mock_->enqueue("Item0");
    mock_->enqueue("3");
    admin_->invokeStock();
    EXPECT_EQ(mock_->sent().back(), "TRUE");
}

TEST_F(ServerAdminTest, StockOrderUnknownItem)
{
    test_workspace::writeSampleGoodsFile();
    test_workspace::writeSampleCashFile();
    mock_->enqueue("1");
    mock_->enqueue("MissingItem");
    admin_->invokeStock();
    EXPECT_EQ(mock_->sent().back(), "FALSE");
}

TEST_F(ServerAdminTest, StockOrderInsufficientCash)
{
    test_workspace::writeFile("cash.txt",
                              "Initial cash: 1\n"
                              "Cash in: 0\n"
                              "Cash out: 0\n"
                              "Final cash: 1\n");
    test_workspace::writeSampleGoodsFile();
    mock_->enqueue("1");
    mock_->enqueue("Item0");
    mock_->enqueue("9999");
    admin_->invokeStock();
    EXPECT_EQ(mock_->sent().back(), "FALSE");
}
