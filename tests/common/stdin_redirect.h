#pragma once

#include <iostream>
#include <sstream>
#include <streambuf>

class StdinRedirect
{
public:
    explicit StdinRedirect(const std::string& input)
        : buffer_(input), backup_(std::cin.rdbuf(buffer_.rdbuf()))
    {
    }

    ~StdinRedirect() { std::cin.rdbuf(backup_); }

private:
    std::istringstream buffer_;
    std::streambuf* backup_;
};
