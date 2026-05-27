#pragma once

#include "Admin.h"
#include "MockThread.h"

class TestableServerAdmin : public Admin
{
public:
    explicit TestableServerAdmin(MockThread& thread) : Admin(thread) {}

    void invokeHome() { home(); }
    void invokeView(const std::string& fileName) { view(fileName); }
    void invokeSearchMenu(const std::string& fileName) { search(fileName); }
    void invokeSearchKey(const std::string& key, const std::string& fileName) { search(key, fileName); }
    void invokeDelete(const std::string& fileName) { Delete(fileName); }
    void invokeBalance(const std::string& fileName) { balance(fileName); }
    void invokeAccounts() { accounts(); }
    void invokeStock() { stock(); }
    void invokeAddEmployee() { add_employee(); }

    void setLookup(const std::string& value) { lookup = value; }
};
