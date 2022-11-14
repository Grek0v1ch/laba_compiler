cmake_minimum_required(VERSION 3.23)
project(laba_compiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(laba_compiler
        src/main.cpp
        src/token/token.cpp
        src/lexical_analyzer/lexical_analyzer.cpp
        src/lexical_analyzer/DFSM.cpp
        src/hash_table/hash_table.cpp
        src/parser/parser.cpp)

target_include_directories(laba_compiler PUBLIC
        src/hash_table
        src/token
        src/lexical_analyzer
        src/parser)
