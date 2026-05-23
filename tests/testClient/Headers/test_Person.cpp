#include <gtest/gtest.h>
#include "Client.h"
#include "test_Person.h"

class ClientPersonValidationTest : public ::testing::Test
{
protected:
    Client client{"127.0.0.1", 18777};
    TestablePerson person{client};
};

TEST_F(ClientPersonValidationTest, ValidName)
{
    EXPECT_TRUE(person.testCheckName("John Smith"));
}

TEST_F(ClientPersonValidationTest, InvalidNameWithDigits)
{
    EXPECT_FALSE(person.testCheckName("John2"));
}

TEST_F(ClientPersonValidationTest, ValidAge)
{
    EXPECT_TRUE(person.testCheckAge(25));
}

TEST_F(ClientPersonValidationTest, AgeTooYoung)
{
    EXPECT_FALSE(person.testCheckAge(5));
}

TEST_F(ClientPersonValidationTest, AgeTooOld)
{
    EXPECT_FALSE(person.testCheckAge(100));
}

TEST_F(ClientPersonValidationTest, ValidSex)
{
    EXPECT_TRUE(person.testCheckSex('M'));
    EXPECT_TRUE(person.testCheckSex('F'));
    EXPECT_TRUE(person.testCheckSex('O'));
}

TEST_F(ClientPersonValidationTest, InvalidSex)
{
    EXPECT_FALSE(person.testCheckSex('X'));
}

TEST_F(ClientPersonValidationTest, ValidCNICPattern)
{
    EXPECT_TRUE(person.testCheckCNIC("12345-1234567-1"));
}

TEST_F(ClientPersonValidationTest, InvalidCNICLength)
{
    EXPECT_FALSE(person.testCheckCNIC("123"));
}

TEST_F(ClientPersonValidationTest, ValidEmail)
{
    EXPECT_TRUE(person.testCheckEmail("user@example.com"));
}

TEST_F(ClientPersonValidationTest, InvalidEmailWithoutAt)
{
    EXPECT_FALSE(person.testCheckEmail("user.example.com"));
}

TEST_F(ClientPersonValidationTest, ValidPhone)
{
    EXPECT_TRUE(person.testCheckPhone("0300-1234567"));
}

TEST_F(ClientPersonValidationTest, InvalidPhoneCharacter)
{
    EXPECT_FALSE(person.testCheckPhone("0300abc"));
}

TEST_F(ClientPersonValidationTest, ValidDOB)
{
    Date dob{15, 6, 2000};
    EXPECT_TRUE(person.testCheckDOB(dob));
}

TEST_F(ClientPersonValidationTest, InvalidDOBMonth)
{
    Date dob{15, 13, 2000};
    EXPECT_FALSE(person.testCheckDOB(dob));
}
