#include <iostream>
#include <memory>

#include "terminal.h"
#include "token.h"
#include "parse_tree.h"

int main() {
    parse_tree tree;
    tree.add_product(terminal::FUNCTION, parse_tree::FUNCTION);
    tree.add_product(terminal::BEGIN, parse_tree::BEGIN);
    tree.add_product(terminal::TYPE, parse_tree::TYPE_1);
    tree.add_product(terminal::FUNCTION_NAME, parse_tree::FUNCTION_NAME);
    tree.add_token(terminal::ID, token{ID, "main"});


    tree.print(std::cout);
}