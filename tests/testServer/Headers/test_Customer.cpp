#include <gtest/gtest.h>

#include <cstring>
#include <memory>

#include "Customer.h"
#include "MockThread.h"
#include "test_workspace.h"

class TestableServerCustomer : public Customer
{
public:
    explicit TestableServerCustomer(MockThread& thread) : Customer(thread) {}

    void invokeStart() { start(); }
    void invokeHome() { home(); }
    void invokeComplain() { complain(); }
    void setUsername(const std::string& value) { username = value; }
    void setLookup(const std::string& value) { lookup = value; }
};

class ServerCustomerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        test_workspace::create("oss_server_customer");
        mock_ = std::make_unique<MockThread>();
        customer_ = std::make_unique<TestableServerCustomer>(*mock_);
    }

    std::unique_ptr<MockThread> mock_;
    std::unique_ptr<TestableServerCustomer> customer_;
};

TEST_F(ServerCustomerTest, CanConstruct)
{
    SUCCEED();
}

TEST_F(ServerCustomerTest, StartExitOption)
{
    mock_->enqueue("3");
    customer_->invokeStart();
    SUCCEED();
}

TEST_F(ServerCustomerTest, HomeProfileOption)
{
    test_workspace::writeFile("customer.txt", test_workspace::sampleUserRecord("cust1"));
    customer_->setLookup("Username: cust1");

    mock_->enqueue("3");
    mock_->enqueue("4");
    customer_->invokeHome();
    ASSERT_FALSE(mock_->sent().empty());
}

TEST_F(ServerCustomerTest, ComplainWriteComplaint)
{
    customer_->setUsername("cust_writer");
    mock_->enqueue("1");
    mock_->enqueue("Product was damaged");
    mock_->enqueue("4");
    customer_->invokeComplain();

    EXPECT_FALSE(mock_->sent().empty());
    EXPECT_TRUE(mock_->sent().back() == "TRUE" || mock_->sent().back() == "FALSE");
}

TEST_F(ServerCustomerTest, SignupCreatesRecord)
{
    mock_->enqueue("1");
    mock_->enqueue("Sign User");
    mock_->enqueue("27");
    mock_->enqueue("F");
    mock_->enqueue("5");
    mock_->enqueue("20");
    mock_->enqueue("1999");
    mock_->enqueue("12345-1234567-1");
    mock_->enqueue("sign@example.com");
    mock_->enqueue("03005556677");
    mock_->enqueue("signuser");
    mock_->enqueue("pass");

    test_workspace::writeFile("customer.txt", "");
    customer_->invokeStart();
    SUCCEED();
}

TEST_F(ServerCustomerTest, HomeBuyPath)
{
    test_workspace::writeSampleGoodsFile();
    test_workspace::writeSampleCashFile();
    test_workspace::writeFile("customer.txt", test_workspace::sampleUserRecord("buy_cust", "secret", "300"));
    customer_->setLookup("Username: buy_cust");

    mock_->enqueue("1");
    mock_->enqueue("1");
    mock_->enqueue("1");
    mock_->enqueue("2");
    mock_->enqueue("3");
    mock_->enqueue("4");
    customer_->invokeHome();
    SUCCEED();
}

TEST_F(ServerCustomerTest, ComplainReviewAndUpdate)
{
    customer_->setUsername("reviewer");
    const std::string complaintFile = "complaint.dat";
    test_workspace::writeFile(complaintFile, "");

    mock_->enqueue("2");
    mock_->enqueue("3");
    mock_->enqueue("Updated complaint");
    mock_->enqueue("4");
    customer_->invokeComplain();
    SUCCEED();
}
