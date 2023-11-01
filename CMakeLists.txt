cmake_minimum_required(VERSION 3.16.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_DEBUG_POSTFIX d)

project(example1)

include_directories(headers)
link_directories(src)

set(HEADER_FILES "headers/input_interface.h" "headers/exit_input.h")
set(SOURCE_FILES "src/main.cpp")

add_executable(example1 ${HEADER_FILES} ${SOURCE_FILES})
