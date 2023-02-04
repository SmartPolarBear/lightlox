cmake_minimum_required(VERSION 3.24)

target_sources(lightlox PRIVATE lexer.cpp)

target_include_directories(lightlox PRIVATE include)