#pragma once

#include <filesystem>
#include <fstream>
#include <string>

#ifdef _WIN32
#include <direct.h>
#define OSS_CHDIR _chdir
#else
#include <unistd.h>
#define OSS_CHDIR chdir
#endif

namespace test_workspace {

inline std::filesystem::path create(const std::string& name)
{
    const auto dir = std::filesystem::temp_directory_path() / name;
    std::filesystem::create_directories(dir);
    OSS_CHDIR(dir.string().c_str());
    return dir;
}

inline void writeFile(const std::string& path, const std::string& content)
{
    std::ofstream out(path, std::ios::trunc);
    out << content;
}

inline std::string sampleUserRecord(const std::string& username,
                                    const std::string& password = "secret",
                                    const std::string& balance = "100")
{
    return "Username: " + username + "\n"
           "Password: " + password + "\n"
           "Name: Test User\n"
           "Age: 25\n"
           "Sex: M\n"
           "Date of birth: 01/15/2000\n"
           "CNIC: 12345-1234567-1\n"
           "Email: test@example.com\n"
           "Phone number: 03001234567\n"
           "Balance: " + balance + "\n\n";
}

inline void writeSampleGoodsFile()
{
    std::string content;
    for (int i = 0; i < 10; ++i)
    {
        content += "Name: Item" + std::to_string(i) + "\n";
        content += "Stock: 10\n";
        content += "Cost: 5\n";
        content += "Price: 10\n";
        content += "Reorder Level: 2\n";
        content += "Reorder Amount: 5\n\n";
    }
    writeFile("goods.txt", content);
}

inline void writeSampleCashFile()
{
    writeFile("cash.txt",
                "Initial: 1000\n"
                "In: 500\n"
                "Out: 200\n"
                "Final: 1300\n");
}

} // namespace test_workspace
