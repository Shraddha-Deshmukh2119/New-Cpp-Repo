#pragma once

#include <ws2tcpip.h>
#include <iostream>
#include <sstream>
#include <string>
#pragma comment(lib, "ws2_32.lib")

class Client 
{
public:
    Client(const std::string &ipAddr = "127.0.0.1", int port = 7777) : m_ipAddress(ipAddr), m_port(port) {}
    ~Client();

    virtual bool start();

    virtual int Send(const std::string& str) const;
    virtual int Send(const int& var) const;
    virtual int Send(const double& var) const;
    virtual int Rec(std::string& receivingString);

private:
    WSAData wsaData;
    SOCKET client;

    const std::string m_ipAddress;
    const int m_port;

    void Connect();
};