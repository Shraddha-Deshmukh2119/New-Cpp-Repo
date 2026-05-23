#include <gtest/gtest.h>

#include <fstream>
#include <memory>

#include "MockThread.h"
#include "test_Person.h"
#include "test_helpers.h"
#include "test_workspace.h"

class ServerPersonTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        test_workspace::create("oss_server_person");
        mock_ = std::make_unique<MockThread>();
        person_ = std::make_unique<TestableServerPerson>(*mock_);
    }

    std::unique_ptr<MockThread> mock_;
    std::unique_ptr<TestableServerPerson> person_;
};

TEST_F(ServerPersonTest, TransferToFileWritesRecord)
{
    const std::string file = "person_transfer.txt";
    EXPECT_TRUE(person_->testTransferToFile(file));

    std::ifstream in(file);
    ASSERT_TRUE(in.is_open());
    std::string line;
    bool foundUser = false;
    while (std::getline(in, line))
    {
        if (line == "Username: testuser")
            foundUser = true;
    }
    EXPECT_TRUE(foundUser);
}

TEST_F(ServerPersonTest, ProfileSendsMatchingRecord)
{
    const std::string file = "profile_data.txt";
    test_workspace::writeFile(file, test_workspace::sampleUserRecord("profile_user"));

    person_->testProfile(file, "Username: profile_user");
    ASSERT_FALSE(mock_->sent().empty());
    EXPECT_NE(mock_->sent().back().find("profile_user"), std::string::npos);
}

TEST_F(ServerPersonTest, InitializeBalanceReadsValue)
{
    const std::string file = "balance_data.txt";
    test_workspace::writeFile(file, test_workspace::sampleUserRecord("payer", "secret", "275"));

    EXPECT_TRUE(person_->testInitializeBalance(file));
    EXPECT_EQ(person_->getBalance(), "275");
}
