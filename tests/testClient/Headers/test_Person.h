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

    void invokeBuy() { buy(); }
    void invokeLogin() { login(); }
    void invokeProfile() { profile(); }
    bool invokeInput() { return input(); }

    // Single-attempt login (production login() retries recursively).
    void invokeLoginOnce()
    {
        system("cls");
        heading("LOGIN");
        cout << " Username : ";
        getline(cin, username);
        fflush(stdin);

        cout << " Password : ";
        char c;
        password.clear();
        while (1)
        {
            c = getch();
            if (c == 13)
                break;
            cout << "*";
            password.push_back(c);
        }

        (*client).Send(username);
        (*client).Send(password);

        string error;
        (*client).Rec(error);

        if (error == "Correct")
        {
            home();
            return;
        }

        if (error == "Incorrect-User")
            cout << "\n Incorrect username" << endl;
        if (error == "Incorrect-Pass")
            cout << "\n Incorrect password" << endl;
    }
};
