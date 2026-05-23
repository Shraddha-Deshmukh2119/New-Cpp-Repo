#pragma once

#include "Person.h"

class TestablePerson : public Person
{
public:
    explicit TestablePerson(Client& client) : Person(client) {}

    void home() override {}

    bool testCheckName(const std::string& name) { return check_name(name); }
    bool testCheckAge(const int& age) { return check_age(age); }
    bool testCheckSex(const char& sex) { return check_sex(sex); }
    bool testCheckDOB(const Date& dob) { return check_DOB(dob); }
    bool testCheckCNIC(const std::string& cnic) { return check_CNIC(cnic); }
    bool testCheckEmail(const std::string& email) { return check_email(email); }
    bool testCheckPhone(const std::string& phone) { return check_phone_num(phone); }
    bool testConsistency(const Date& dob, const int& age) { return consistency(dob, age); }
};
