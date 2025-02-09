cmake_minimum_required(VERSION 3.10)
project(MachineCodeMultiProcessingSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# Boost
find_package(Boost REQUIRED COMPONENTS system thread)

# OpenSSL
find_package(OpenSSL REQUIRED)

# Server executable
add_executable(server
    server/main.cpp
    server/auth.cpp
    server/distribute.cpp
    server/monitoring.cpp
    server/security.cpp
    server/task_manager.cpp
    server/virtual_cpu.cpp
    server/virtual_ram.cpp
    server/virtual_gpu.cpp
    server/tcp_ip.cpp
    server/websocket.cpp
    server/encryptor.cpp
    server/error_handler.cpp
)
target_link_libraries(server OpenSSL::SSL OpenSSL::Crypto Boost::system Boost::thread pthread)

# Client executable
add_executable(client
    client/worker.cpp
    client/auth.cpp
    client/connection.cpp
    client/execution.cpp
    client/gpu_support.cpp
)
target_link_libraries(client OpenSSL::SSL OpenSSL::Crypto Boost::system Boost::thread pthread)

# Test executables
add_executable(test_server tests/test_server.cpp)
target_link_libraries(test_server OpenSSL::SSL OpenSSL::Crypto Boost::system Boost::thread pthread)

add_executable(test_client tests/test_client.cpp)
target_link_libraries(test_client OpenSSL::SSL OpenSSL::Crypto Boost::system Boost::thread pthread)

add_executable(test_security tests/test_security.cpp)
target_link_libraries(test_security OpenSSL::SSL OpenSSL::Crypto Boost::system Boost::thread pthread)

# Include directories
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR})
