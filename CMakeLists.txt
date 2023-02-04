cmake_minimum_required(VERSION 3.24)
project(lightlox)

set(CMAKE_CXX_STANDARD 23)

add_executable(lightlox main.cpp)

add_subdirectory(lightlox)

add_subdirectory(lexer)

