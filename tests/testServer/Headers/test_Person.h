#pragma once

#include "Person.h"
#include "MockThread.h"

class TestableServerPerson : public Person
{
public:
    explicit TestableServerPerson(MockThread& server) : Person(server) {}

    void home() override {}

    bool testTransferToFile(const std::string& fileName)
    {
        username = "testuser";
        password = "secret";
        name = "Test User";
        age = "30";
        sex = "M";
        DOB.month = "01";
        DOB.day = "15";
        DOB.year = "1995";
        CNIC = "12345-1234567-1";
        email = "test@example.com";
        phone_num = "03001234567";
        return transfer_to_file(fileName);
    }

    void testProfile(const std::string& fileName, const std::string& lookup)
    {
        profile(fileName, lookup);
    }

    bool testInitializeBalance(const std::string& fileName)
    {
        lookup = "Username: payer";
        return initialize_balance(fileName);
    }

    void setLookup(const std::string& value) { lookup = value; }
    std::string getBalance() const { return balance; }
};
