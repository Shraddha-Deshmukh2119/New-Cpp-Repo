#pragma once

#include <queue>
#include <sstream>
#include <string>
#include <vector>

#include "Thread.h"

class MockThread : public Thread
{
public:
    void enqueue(const std::string& message) { inbox_.push(message); }

    const std::vector<std::string>& sent() const { return sent_; }
    void clearSent() { sent_.clear(); }

    int Send(const std::string& str) const override
    {
        sent_.push_back(str);
        return static_cast<int>(str.size() + 1);
    }

    int Send(const int& var) const override
    {
        std::ostringstream ss;
        ss << var;
        return Send(ss.str());
    }

    int Send(const double& var) const override
    {
        std::ostringstream ss;
        ss << var;
        return Send(ss.str());
    }

    int Rec(std::string& receivingString) override
    {
        if (inbox_.empty())
        {
            receivingString.clear();
            return 0;
        }
        receivingString = inbox_.front();
        inbox_.pop();
        return static_cast<int>(receivingString.size());
    }

private:
    mutable std::queue<std::string> inbox_;
    mutable std::vector<std::string> sent_;
};
