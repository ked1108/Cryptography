cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(LAB1)

add_executable(server server.cpp)
add_executable(client client.cpp)

add_library(encrypt encrypt.cpp)
target_link_libraries(server PUBLIC encrypt)
target_link_libraries(client PUBLIC encrypt)

