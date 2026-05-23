cmake_minimum_required(VERSION 3.16)
project(OnlineShoppingSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_APPS "Build client and server applications" ON)
option(BUILD_TESTS "Build unit tests" ON)
option(ENABLE_COVERAGE "Enable code coverage flags (GCC/Clang)" OFF)

if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
    add_link_options(--coverage)
endif()

if(BUILD_APPS)
    add_executable(client_app Client/clientProgram.cpp)
    target_include_directories(client_app PRIVATE Client Client/Headers)
    target_sources(client_app PRIVATE
        Client/Headers/Client.cpp
        Client/Headers/Person.cpp
        Client/Headers/Customer.cpp
        Client/Headers/Employee.cpp
        Client/Headers/Admin.cpp
    )
    target_link_libraries(client_app PRIVATE ws2_32)

    add_executable(server_app Server/serverProgram.cpp)
    target_include_directories(server_app PRIVATE Server Server/Headers)
    target_sources(server_app PRIVATE
        Server/Headers/Server.cpp
        Server/Headers/Thread.cpp
        Server/Headers/Person.cpp
        Server/Headers/Customer.cpp
        Server/Headers/Employee.cpp
        Server/Headers/Admin.cpp
        Server/Headers/Complaint_Base.cpp
        Server/Headers/Complaint_E.cpp
    )
    target_link_libraries(server_app PRIVATE ws2_32)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
