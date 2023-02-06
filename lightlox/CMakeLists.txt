cmake_minimum_required(VERSION 3.24)

target_include_directories(lightlox PRIVATE include)

target_sources(lightlox PRIVATE io.cpp)