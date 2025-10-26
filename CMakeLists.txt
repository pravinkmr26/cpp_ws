cmake_minimum_required(VERSION 3.31)
project(cppbasics)

set(CMAKE_CXX_STANDARD 20)

add_executable(cppbasics
        another_awaitable_coroutine.cpp
        my_smart_pointer.cpp
        access_private_members.cpp
        sudo_lib.cpp
        ex_sudo1.cpp
        ThreadPool.cpp
        tcp_server.cpp
        tcp_client.cpp
        reference_tcp_server.cpp
        reference_tcp_client.cpp)
