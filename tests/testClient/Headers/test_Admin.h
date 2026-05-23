#pragma once

#include "Admin.h"
#include "MockClient.h"

class TestableClientAdmin : public Admin
{
public:
    explicit TestableClientAdmin(MockClient& client) : Admin(client) {}

    void invokeView(const std::string& head) { view(head); }
    void invokeSearchMenu() { search(); }
    void invokeSearchKey(const std::string& key) { search(key); }
    void invokeDelete() { Delete(); }
    void invokeBalance() { balance(); }
    void invokeStock() { stock(); }
    void invokeAccounts() { accounts(); }
    void invokeHomeExit() { home(); }
};
