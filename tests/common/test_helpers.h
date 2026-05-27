#pragma once

#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <string>

namespace test_helpers {

inline int testPort()
{
    if (const char* env = std::getenv("TEST_PORT"))
        return std::atoi(env);
    return 18777;
}

inline std::string tempFile(const std::string& name)
{
    const auto dir = std::filesystem::temp_directory_path() / "oss_tests";
    std::filesystem::create_directories(dir);
    return (dir / name).string();
}

inline void removeIfExists(const std::string& path)
{
    std::error_code ec;
    std::filesystem::remove(path, ec);
}

// January 1 DOB so age matches year arithmetic regardless of current month/day.
inline std::string dobStringForAge(int age)
{
    const std::time_t now = std::time(nullptr);
    const std::tm* t = std::localtime(&now);
    const int year = (t->tm_year + 1900) - age;
    return "1/1/" + std::to_string(year);
}

// Stdin for Client::Person::input() / Admin::add_employee() (password via wrapped getch).
inline std::string clientPersonInputStdin(const std::string& username,
                                          const std::string& name,
                                          int age,
                                          const std::string& email = "user@example.com",
                                          const std::string& phone = "03001234567")
{
    return username + "\n" + name + "\n" + std::to_string(age) + "\nM\n" + dobStringForAge(age) +
           "\n12345-1234567-1\n" + email + "\n" + phone + "\n";
}

} // namespace test_helpers
