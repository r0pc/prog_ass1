cmake_minimum_required(VERSION 3.10)
project(prog_ass1 C)

set(CMAKE_C_STANDARD 11)

file(GLOB SRC_FILES "src/*.c")

add_executable(prog_ass1 ${SRC_FILES})

target_include_directories(prog_ass1 PRIVATE src)
