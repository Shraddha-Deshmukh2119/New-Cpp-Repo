#pragma once
#include "Person.h"

class Customer: public Person
{
protected:
    void signup();
    void home() override;
    void complain();

public:
    Customer(Thread& server) : Person(server) {}
    void start();
};
