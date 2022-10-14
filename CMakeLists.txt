cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(laba)
add_compile_options(-std=c++11)
add_compile_options(-g)
add_compile_options(-Wall -Wextra -pedantic -Werror)

add_executable(laba src/main.cpp)