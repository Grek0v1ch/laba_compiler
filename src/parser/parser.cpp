#include "parser.h"

void parser::check_lexical_error(token& curr) {
    if (curr.type() == UNKNOWN && ! curr.text().empty()) {
        std::cout << "Unknown lexeme " << curr.text() << '\n';
    }
}

bool parser::run() {
    _parse_tree.add_product(terminal::FUNCTION, parse_tree::FUNCTION);

    parse_tree begin_tree(terminal::BEGIN);
    if (! begin(begin_tree)) {
        return false;
    }
    _parse_tree.insert_tree(terminal::BEGIN, begin_tree);

    parse_tree description_tree(terminal::DESCRIPTION);
    if (! description(description_tree)) {
        return false;
    }
    _parse_tree.insert_tree(terminal::DESCRIPTION, description_tree);

    parse_tree operators_tree(terminal::OPERATORS);
    if (! operators(operators_tree)) {
        return false;
    }
    _parse_tree.insert_tree(terminal::OPERATORS, operators_tree);

    parse_tree end_tree(terminal::END);
    if (! end(end_tree)) {
        return false;
    }
    _parse_tree.insert_tree(terminal::END, end_tree);

    token curr = _tokens.get_next_token();
    if (curr.type() == UNKNOWN && curr.text().empty()) {
        return true;
    }
    position curr_pos = _tokens.position();
    std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
    std::cout << "Error: no more one function\n";
    return false;
}

bool parser::begin(parse_tree& begin_tree) {
    begin_tree.add_product(terminal::BEGIN, parse_tree::BEGIN);

    position curr_pos = _tokens.position();
    token curr = _tokens.get_next_token();
    if (curr.type() != INT && curr.type() != CHAR) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in the type of function: the type should be int or char\n";
        return false;
    }

    if (curr.type() == INT) {
        begin_tree.add_product(terminal::TYPE, parse_tree::TYPE_1);
    } else if (curr.type() == CHAR) {
        begin_tree.add_product(terminal::TYPE, parse_tree::TYPE_2);
    }

    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.type() != ID) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in the function name: the name must be the correct identifier\n";
        return false;
    }

    begin_tree.add_product(terminal::FUNCTION_NAME, parse_tree::FUNCTION_NAME);
    begin_tree.add_token(terminal::ID, curr);

    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.type() != LBRACKET) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error int the function description: not find (\n";
        return false;
    }
    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.type() != RBRACKET) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error int the function description: not find )\n";
        return false;
    }
    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.type() != LBRACKET_FIGURE) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error int the function description: not find {\n";
        return false;
    }
    return true;
}

bool parser::description(parse_tree& description_tree) {

    parse_tree small_description_tree(terminal::DESCR);

    if (! small_descriptions(small_description_tree)) {
        return false;
    }
    while (true) {
        token curr = _tokens.get_next_token();
        if (curr.type() != INT && curr.type() != CHAR) {
            _tokens.return_last_word();

            description_tree.add_tree(terminal::DESCRIPTION, small_description_tree);

            return true;
        }

        description_tree.add_product(terminal::DESCRIPTION, parse_tree::DESCRIPTION_2);
        description_tree.insert_tree(terminal::DESCR, small_description_tree);
        small_description_tree.clear();

        _tokens.return_last_word();
        if (! small_descriptions(small_description_tree)) {
            return false;
        }
    }
}

bool parser::small_descriptions(parse_tree& small_description_tree) {

    small_description_tree.add_product(terminal::DESCR, parse_tree::DESCR);

    position curr_pos = _tokens.position();
    token curr = _tokens.get_next_token();
    if (curr.type() != INT && curr.type() != CHAR) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in the description: "
                  << "the description should start with the type of variable\n";
    }

    if (curr.type() == INT) {
        small_description_tree.add_product(terminal::TYPE, parse_tree::TYPE_1);
    } else if (curr.type() == CHAR) {
        small_description_tree.add_product(terminal::TYPE, parse_tree::TYPE_2);
    }
    parse_tree var_list_tree(terminal::VAR_LIST);

    if (! var_list(var_list_tree)) {
        return false;
    }

    small_description_tree.insert_tree(terminal::VAR_LIST, var_list_tree);

    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.type() != SEMICOLON) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "error in the descriptions ending: no ;\n";
        return false;
    }
    return true;
}

bool parser::var_list(parse_tree& var_list_tree) {
    position curr_pos = _tokens.position();
    token curr = _tokens.get_next_token();
    if (curr.type() != ID) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in the description: no identifier in description\n";
        return false;
    }
    token prev = curr;
    curr = _tokens.get_next_token();
    if (curr.type() != COMMA) {
        _tokens.return_last_word();

        var_list_tree.add_product(terminal::VAR_LIST, parse_tree::VAR_LIST_1);
        var_list_tree.add_token(terminal::ID, prev);

        return true;
    }

    var_list_tree.add_product(terminal::VAR_LIST, parse_tree::VAR_LIST_2);
    var_list_tree.add_token(terminal::ID, prev);

    return var_list(var_list_tree);
}

bool parser::operators(parse_tree& operators_tree) {

    parse_tree small_operators_tree(terminal::OP);

    if (! small_operators(small_operators_tree)) {
        return false;
    }
    while (true) {
        token curr = _tokens.get_next_token();
        if (curr.type() != ID) {
            _tokens.return_last_word();

            operators_tree.add_tree(terminal::OPERATORS, small_operators_tree);

            return true;
        }

        operators_tree.add_product(terminal::OPERATORS, parse_tree::OPERATORS_2);
        operators_tree.insert_tree(terminal::OP, small_operators_tree);
        small_operators_tree.clear();

        _tokens.return_last_word();
        if (! small_operators(small_operators_tree)) {
            return false;
        }
    }
}

bool parser::small_operators(parse_tree& small_operators_tree) {
    position curr_pos = _tokens.position();
    token curr = _tokens.get_next_token();
    if (curr.type() != ID) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in operators: expression must be assigned to the variable\n";
        return false;
    }
    token id_token = curr;
    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.type() != EQUALS) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in operators: not find =\n";
        return false;
    }
    curr = _tokens.get_next_token();
    if (curr.type() == ID || curr.type() == NUMBER || curr.type() == LBRACKET) {
        _tokens.return_last_word();

        small_operators_tree.add_product(terminal::OP, parse_tree::OP_1);
        small_operators_tree.add_token(terminal::ID, id_token);
        parse_tree num_expr_tree(terminal::NUM_EXPR);

        if (! num_expr(num_expr_tree)) {
            return false;
        }

        small_operators_tree.insert_tree(terminal::NUM_EXPR, num_expr_tree);

    } else if (curr.type() == SIMPLE_STRING) {
        _tokens.return_last_word();

        small_operators_tree.add_product(terminal::OP, parse_tree::OP_2);
        small_operators_tree.add_token(terminal::ID, id_token);
        parse_tree string_expr_tree(terminal::STRING_EXPR);

        if (! string_expr(string_expr_tree)) {
            return false;
        }

        small_operators_tree.insert_tree(terminal::STRING_EXPR, string_expr_tree);

    } else {
        return false;
    }
    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.type() != SEMICOLON) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in operators: not find ;\n";
        return false;
    }
    return true;
}

bool parser::num_expr(parse_tree& num_expr_tree) {

    parse_tree simple_num_expr_tree(terminal::SIMPLE_NUM_EXPR);

    if (! simple_num_expr(simple_num_expr_tree)) {
        return false;
    }
    token curr = _tokens.get_next_token();
    if (curr.type() != SUM && curr.type() != MINUS) {
        _tokens.return_last_word();

        num_expr_tree.add_tree(terminal::NUM_EXPR, simple_num_expr_tree);

        return true;
    }

    if (curr.type() == SUM) {
        num_expr_tree.add_product(terminal::NUM_EXPR, parse_tree::NUM_EXPR_2);
        num_expr_tree.insert_tree(terminal::SIMPLE_NUM_EXPR, simple_num_expr_tree);
    } else if (curr.type() == MINUS) {
        num_expr_tree.add_product(terminal::NUM_EXPR, parse_tree::NUM_EXPR_3);
        num_expr_tree.insert_tree(terminal::SIMPLE_NUM_EXPR, simple_num_expr_tree);
    }

    return num_expr(num_expr_tree);
}

bool parser::simple_num_expr(parse_tree& simple_num_expr_tree) {
    position curr_pos = _tokens.position();
    token curr = _tokens.get_next_token();
    if (curr.type() == LBRACKET) {

        simple_num_expr_tree.add_product(terminal::SIMPLE_NUM_EXPR, parse_tree::SIMPLE_NUM_EXPR_3);
        parse_tree num_expr_tree(terminal::NUM_EXPR);

        if (! num_expr(num_expr_tree)) {
            return false;
        }

        simple_num_expr_tree.insert_tree(terminal::NUM_EXPR, num_expr_tree);

        curr_pos = _tokens.position();
        curr = _tokens.get_next_token();
        if (curr.type() != RBRACKET) {
            check_lexical_error(curr);
            std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
            std::cout << "Error in simple number expression: not find )\n";
            return false;
        }
        return true;
    }
    if (curr.type() != ID && curr.type() != NUMBER) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in simple number expression: " << curr.text()
                  << " must be a number or a variable\n";
        return false;
    }

    if (curr.type() == ID) {
        simple_num_expr_tree.add_product(terminal::SIMPLE_NUM_EXPR, parse_tree::SIMPLE_NUM_EXPR_1);
        simple_num_expr_tree.add_token(terminal::ID, curr);
    } else if (curr.type() == NUMBER) {
        simple_num_expr_tree.add_product(terminal::SIMPLE_NUM_EXPR, parse_tree::SIMPLE_NUM_EXPR_2);
        simple_num_expr_tree.add_token(terminal::CONST, curr);
    }

    return true;
}

bool parser::string_expr(parse_tree& string_expr_tree) {
    token curr = _tokens.get_next_token();
    if (curr.type() != SIMPLE_STRING) {
        position curr_pos = _tokens.position();
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() - curr.text().size() << ' ';
        std::cout << "Error in number expression: " << curr.text() << " must be a string expression\n";
        return false;
    }
    token prev = curr;
    curr = _tokens.get_next_token();
    if (curr.type() != SUM) {
        _tokens.return_last_word();

        string_expr_tree.add_product(terminal::STRING_EXPR, parse_tree::STRING_EXPR_1);
        string_expr_tree.add_token(terminal::SIMPLE_STRING_EXPR, prev);

        return true;
    }

    string_expr_tree.add_product(terminal::STRING_EXPR, parse_tree::STRING_EXPR_2);
    string_expr_tree.add_token(terminal::SIMPLE_STRING_EXPR, prev);

    return string_expr(string_expr_tree);
}

bool parser::end(parse_tree& end_tree) {

    end_tree.add_product(terminal::END, parse_tree::END);

    position curr_pos = _tokens.position();
    token curr = _tokens.get_next_token();
    if (curr.type() != RETURN) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "error in the function ending: no keyword return\n";
        return false;
    }
    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.type() != ID) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "error in the function ending: no identifier\n";
        return false;
    }

    end_tree.add_token(terminal::ID, curr);

    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.type() != SEMICOLON) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "error in the function ending: no ;\n";
        return false;
    }
    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.type() != RBRACKET_FIGURE) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "error in the function ending: no }\n";
        return false;
    }
    return true;
}
