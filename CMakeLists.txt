cmake_minimum_required(VERSION 3.24)

project(lightlox)

set(CMAKE_CXX_STANDARD 23)

add_executable(lightlox main.cpp)

add_subdirectory(libs)

target_link_libraries(lightlox PRIVATE argparse
        PRIVATE magic_enum)

add_subdirectory(lightlox)

add_subdirectory(lexer)
add_subdirectory(parser)
