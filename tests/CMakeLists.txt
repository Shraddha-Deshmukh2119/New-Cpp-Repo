set(GTEST_ROOT "${CMAKE_BINARY_DIR}/_deps/googletest-src")
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

if(EXISTS "${GTEST_ROOT}/CMakeLists.txt")
    add_subdirectory(${GTEST_ROOT} ${CMAKE_BINARY_DIR}/_deps/googletest-build EXCLUDE_FROM_ALL)
else()
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(googletest)
endif()

add_library(oss_test_support STATIC common/test_runtime.cpp)

set(CLIENT_PROD_SOURCES
    ${CMAKE_SOURCE_DIR}/Client/Headers/Client.cpp
    ${CMAKE_SOURCE_DIR}/Client/Headers/Person.cpp
    ${CMAKE_SOURCE_DIR}/Client/Headers/Customer.cpp
    ${CMAKE_SOURCE_DIR}/Client/Headers/Employee.cpp
    ${CMAKE_SOURCE_DIR}/Client/Headers/Admin.cpp
)

set(SERVER_PROD_SOURCES
    ${CMAKE_SOURCE_DIR}/Server/Headers/Server.cpp
    ${CMAKE_SOURCE_DIR}/Server/Headers/Thread.cpp
    ${CMAKE_SOURCE_DIR}/Server/Headers/Person.cpp
    ${CMAKE_SOURCE_DIR}/Server/Headers/Customer.cpp
    ${CMAKE_SOURCE_DIR}/Server/Headers/Employee.cpp
    ${CMAKE_SOURCE_DIR}/Server/Headers/Admin.cpp
    ${CMAKE_SOURCE_DIR}/Server/Headers/Complaint_Base.cpp
    ${CMAKE_SOURCE_DIR}/Server/Headers/Complaint_E.cpp
)

set(TEST_CLIENT_SOURCES
    testClient/Headers/test_Cash.cpp
    testClient/Headers/test_Client.cpp
    testClient/Headers/test_Person.cpp
    testClient/Headers/test_Person_flows.cpp
    testClient/Headers/test_Customer.cpp
    testClient/Headers/test_Customer_flows.cpp
    testClient/Headers/test_Employee.cpp
    testClient/Headers/test_Employee_flows.cpp
    testClient/Headers/test_Admin.cpp
)

set(TEST_SERVER_SOURCES
    testServer/Headers/test_Cash.cpp
    testServer/Headers/test_Server.cpp
    testServer/Headers/test_Thread.cpp
    testServer/Headers/test_Person.cpp
    testServer/Headers/test_Person_flows.cpp
    testServer/Headers/test_Customer.cpp
    testServer/Headers/test_Employee.cpp
    testServer/Headers/test_Admin.cpp
    testServer/Headers/test_Complaint_Base.cpp
    testServer/Headers/test_Complaint_E.cpp
    testServer/Headers/test_Complaint_C.cpp
)

add_executable(testClient ${TEST_CLIENT_SOURCES} ${CLIENT_PROD_SOURCES})
target_include_directories(testClient PRIVATE
    ${CMAKE_SOURCE_DIR}/Client
    ${CMAKE_SOURCE_DIR}/Client/Headers
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/testClient/Headers
)
target_link_libraries(testClient PRIVATE GTest::gtest GTest::gtest_main ws2_32 oss_test_support)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_link_options(testClient PRIVATE
        "-Wl,--wrap=getch"
        "-Wl,--wrap=_getch"
        "-Wl,--wrap=system"
    )
endif()
add_test(NAME testClient COMMAND testClient)

add_executable(testServer ${TEST_SERVER_SOURCES} ${SERVER_PROD_SOURCES})
target_include_directories(testServer PRIVATE
    ${CMAKE_SOURCE_DIR}/Server
    ${CMAKE_SOURCE_DIR}/Server/Headers
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/testServer/Headers
)
target_link_libraries(testServer PRIVATE GTest::gtest GTest::gtest_main ws2_32 oss_test_support)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_link_options(testServer PRIVATE
        "-Wl,--wrap=getch"
        "-Wl,--wrap=_getch"
        "-Wl,--wrap=system"
    )
endif()
add_test(NAME testServer COMMAND testServer)

if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_custom_target(coverage
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage
        COMMAND gcovr -r ${CMAKE_SOURCE_DIR} --object-directory ${CMAKE_BINARY_DIR}
                --filter "Client/" --filter "Server/"
                --xml ${CMAKE_BINARY_DIR}/coverage.xml
                --html-details ${CMAKE_BINARY_DIR}/coverage/index.html
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating coverage.xml and HTML report (open build/coverage/index.html)"
        VERBATIM
    )
endif()
