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

    tree.add_product(terminal::DESCRIPTION, parse_tree::DESCRIPTION_1);
    tree.add_product(terminal::DESCR, parse_tree::DESCR);
    tree.add_product(terminal::TYPE, parse_tree::TYPE_1);
    tree.add_product(terminal::VAR_LIST, parse_tree::VAR_LIST_1);
    tree.add_token(terminal::ID, token{ID, "a"});

    tree.add_product(terminal::OPERATORS, parse_tree::OPERATORS_1);
    tree.add_product(terminal::OP, parse_tree::OP_1);
    tree.add_token(terminal::ID, token{ID, "a"});
    tree.add_product(terminal::NUM_EXPR, parse_tree::NUM_EXPR_1);
    tree.add_product(terminal::SIMPLE_NUM_EXPR, parse_tree::SIMPLE_NUM_EXPR_2);
    tree.add_token(terminal::CONST, token{NUMBER, "1"});

    tree.add_product(terminal::END, parse_tree::END);
    tree.add_token(terminal::ID, token{ID, "a"});

    tree.print(std::cout);
}