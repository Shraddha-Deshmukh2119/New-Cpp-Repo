#pragma once

#include "Employee.h"
#include "MockClient.h"

class TestableClientEmployee : public Employee
{
public:
    explicit TestableClientEmployee(MockClient& client) : Employee(client) {}

    void invokeHomeExit() { home(); }
    void invokeHome() { home(); }
    void invokeComplainExit() { complain(); }
    void invokeComplain() { complain(); }
};
