#pragma once

#include "Customer.h"
#include "MockClient.h"

class TestableClientCustomer : public Customer
{
public:
    explicit TestableClientCustomer(MockClient& client) : Customer(client) {}

    void invokeStartBack() { start(); }
    void invokeStart() { start(); }
    void invokeSignup() { signup(); }
    void invokeHomeLogout() { home(); }
    void invokeHome() { home(); }
    void invokeComplain() { complain(); }
};
