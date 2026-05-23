#pragma once

#include <cstdlib>
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

} // namespace test_helpers
