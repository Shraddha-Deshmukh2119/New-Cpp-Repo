#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <conio.h>
#include <cstdio>
#include <cstring>
#include "Client.h"

using namespace std;

// N is the number of goods we sell.
#define N (10)

// Forward declaration of classes
class Person;
// class Employee;
class Customer;
// class Admin;

// Non-member functions and variables:
inline void welcome(void)
{
    cout << setw(60) << "WELCOME TO MSM GROCERY CENTER!" << endl << endl;
}
inline void heading(const string& head)
{
    cout << setw(40) << "" << head << endl << endl;
}

struct Date
{
    int day, month, year;
};

class Person
{
    protected:
        string name, CNIC, email, phone_num, username, password;
        char sex;
        int age;
        double balance = 0;
        Date DOB;
        Client* client;

        bool input();
        void input_name(string& name);
        void input_age(int& age);
        void input_sex(char& sex);
        void input_DOB(Date& DOB, const int& myAge);
        void input_CNIC(string& CNIC);
        void input_email(string& email);
        void input_phone_num(string& phone_num);
        void input_username(string& username);
        void input_password(string& password);
        void transfer();

        // Method for validation
        bool check_name(const string& name);
        bool check_age (const int& age);
        bool check_sex (const char& sex);
        bool check_DOB (const Date& DOB);
        bool check_CNIC(const string& CNIC);
        bool check_email(const string& email);
        bool check_phone_num(const string& phone_num);
        bool check_password(const string& password);
        bool check_username(const string& username); // no definition provided
        
        // Returns true if entered date of birth is consistent with entered age
        bool consistency(const Date& DOB, const int& myAge);
        // pure virtual function
        virtual void home() = 0; 

        // Other methods
        void profile();
        void buy();

    public:
        Person(Client& client) : client(&client) {};
        void login();   
};
