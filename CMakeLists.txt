cmake_minimum_required(VERSION 3.23)
project(laba_compiler)

set(CMAKE_CXX_STANDARD 11)

add_executable(laba_compiler
        src/main.cpp
        src/lexical_item/token.cpp
        src/lexical_analyzer/lexical_analyzer.cpp
        src/lexical_analyzer/DFSM.cpp
        src/hash_table/hash_table.cpp
        src/parser/parser.cpp)

target_include_directories(laba_compiler PUBLIC
        src/hash_table
        src/lexical_item
        src/lexical_analyzer
        src/parser)

add_executable(tree_test
        src/lexical_item/token.cpp
        src/parse_tree/main.cpp
        src/parse_tree/parse_tree.cpp
        src/lexical_item/terminal.cpp)

target_include_directories(tree_test PUBLIC
        src/lexical_item
        src/parse_tree)
