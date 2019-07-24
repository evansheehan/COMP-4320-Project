cmake_minimum_required(VERSION 3.13)
project(Project_1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Project_1_Client
        client.c)

add_executable(Project_1_Server
        server.c)
