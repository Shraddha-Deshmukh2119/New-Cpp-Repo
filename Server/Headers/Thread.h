#pragma once

#include <string>
#include <windows.h>

class Server;
struct Data
{
    Server *server;
    SOCKET client;
    int index;
};
void worker(Data *);

class Thread
{
public:
    Thread() = default;
    Thread(Server* server, int index)
    {
        data.server = server;
        data.index = index;;
    }

    void create();
    bool available() const;
    void enter(SOCKET &Client);
    void setFree();
    bool isFree() const;
    bool isDestroyed() const;
    void CloseThread();
    HANDLE getHandle() const;
    SOCKET getClient() const;
    void endServer();

    // extra function:
    std::string getActivity();
    void closeClient();
 
    virtual int Send(const std::string& str) const;
    virtual int Send(const int& var) const;
    virtual int Send(const double& var) const;
    virtual int Rec(std::string& receivingString);


private:
    HANDLE handle = nullptr;
    Data data;
    bool free = true;
    bool destroyed = false;
};