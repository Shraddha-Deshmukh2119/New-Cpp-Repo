#pragma once

#include <fstream>

#include "Cash.h"
#include "Goods.h"
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

    void invokeLogin(const std::string& fileName) { login(fileName); }

    void invokeLoginOnce(const std::string& fileName)
    {
        (*server).Rec(username);
        (*server).Rec(password);

        ifstream file(fileName);
        string line;
        bool userFound = false, passFound = false;

        if (!file.is_open())
            return;

        while (!file.eof())
        {
            getline(file, line);
            if (line == ("Username: " + username))
            {
                userFound = true;
                getline(file, line);
                if (line == ("Password: " + password))
                    passFound = true;
            }
        }
        file.close();

        if (userFound && passFound)
        {
            lookup = "Username: " + username;
            (*server).Send("Correct");
            home();
            return;
        }
        if (!userFound && !passFound)
            (*server).Send("Incorrect-User");
        if (userFound && !passFound)
            (*server).Send("Incorrect-Pass");
    }
    void invokeBuy(const std::string& fileName) { buy(fileName); }

    bool testInput(const std::string& fileName) { return input(fileName); }

    bool testUpdateBalance(const std::string& fileName)
    {
        balance = "150";
        return update_balance(fileName);
    }

    bool testUpdateGoods()
    {
        Goods goods[N];
        if (!initialize_goods(goods))
            return false;
        return update_goods(goods);
    }

    bool testUpdateCash()
    {
        Cash cash;
        if (!initialize_cash(cash))
            return false;
        cash + 10.0;
        return update_cash(cash);
    }
};
