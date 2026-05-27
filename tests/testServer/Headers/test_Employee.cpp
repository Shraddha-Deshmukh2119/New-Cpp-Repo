#include <gtest/gtest.h>

#include <memory>

#include "Employee.h"
#include "MockThread.h"
#include "test_workspace.h"

class TestableServerEmployee : public Employee
{
public:
    explicit TestableServerEmployee(MockThread& thread) : Employee(thread) {}

    void invokeHome() { home(); }
    void invokeComplain() { complain(); }
    void setLookup(const std::string& value) { lookup = value; }
};

class ServerEmployeeTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        test_workspace::create("oss_server_employee");
        mock_ = std::make_unique<MockThread>();
        employee_ = std::make_unique<TestableServerEmployee>(*mock_);
    }

    std::unique_ptr<MockThread> mock_;
    std::unique_ptr<TestableServerEmployee> employee_;
};

TEST_F(ServerEmployeeTest, CanConstruct)
{
    SUCCEED();
}

TEST_F(ServerEmployeeTest, HomeProfileOption)
{
    test_workspace::writeFile("emp.txt", test_workspace::sampleUserRecord("emp1"));
    employee_->setLookup("Username: emp1");

    mock_->enqueue("3");
    mock_->enqueue("4");
    employee_->invokeHome();

    ASSERT_FALSE(mock_->sent().empty());
    EXPECT_NE(mock_->sent().back().find("emp1"), std::string::npos);
}

TEST_F(ServerEmployeeTest, ComplainViewAll)
{
    const std::string complaintFile = "complaint.dat";
    test_workspace::writeFile(complaintFile, "");

    mock_->enqueue("5");
    mock_->enqueue("7");
    employee_->invokeComplain();
    SUCCEED();
}

TEST_F(ServerEmployeeTest, HomeBuyPath)
{
    test_workspace::writeSampleGoodsFile();
    test_workspace::writeSampleCashFile();
    test_workspace::writeFile("emp.txt", test_workspace::sampleUserRecord("emp_buyer", "secret", "400"));
    employee_->setLookup("Username: emp_buyer");

    mock_->enqueue("1");
    mock_->enqueue("1");
    mock_->enqueue("1");
    mock_->enqueue("2");
    mock_->enqueue("3");
    mock_->enqueue("4");
    employee_->invokeHome();
    SUCCEED();
}

TEST_F(ServerEmployeeTest, ComplainSeeNamedFound)
{
    test_workspace::writeFile("complaint.dat", "");
    mock_->enqueue("1");
    mock_->enqueue("Alice");
    mock_->enqueue("7");
    employee_->invokeComplain();
    SUCCEED();
}

TEST_F(ServerEmployeeTest, ComplainSeeAnonymous)
{
    test_workspace::writeFile("complaint.dat", "");
    mock_->enqueue("2");
    mock_->enqueue("Bob");
    mock_->enqueue("7");
    employee_->invokeComplain();
    SUCCEED();
}

TEST_F(ServerEmployeeTest, ComplainAnswerComplaint)
{
    test_workspace::writeFile("complaint.dat", "");
    mock_->enqueue("3");
    mock_->enqueue("Carol");
    mock_->enqueue("We apologize");
    mock_->enqueue("7");
    employee_->invokeComplain();
    SUCCEED();
}

TEST_F(ServerEmployeeTest, ComplainViewComplaintees)
{
    test_workspace::writeFile("complaint.dat", "");
    mock_->enqueue("4");
    mock_->enqueue("7");
    employee_->invokeComplain();
    SUCCEED();
}

TEST_F(ServerEmployeeTest, ComplainClearAll)
{
    test_workspace::writeFile("complaint.dat", "");
    mock_->enqueue("6");
    mock_->enqueue("7");
    employee_->invokeComplain();
    SUCCEED();
}

TEST_F(ServerEmployeeTest, ComplainInvalidChoice)
{
    mock_->enqueue("99");
    mock_->enqueue("7");
    employee_->invokeComplain();
    SUCCEED();
}
