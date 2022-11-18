#include "parser.h"

void parser::check_lexical_error(token& curr) {
    if (curr.type() == UNKNOWN && ! curr.text().empty()) {
        std::cout << "Unknown lexeme " << curr.text() << '\n';
    }
}

bool parser::run() {
    if (! begin()) {
        return false;
    }
    if (! description()) {
        return false;
    }
    if (! operators()) {
        return false;
    }
    if (! end()) {
        return false;
    }
    token curr = _tokens.get_next_token();
    if (curr.type() == UNKNOWN && curr.text().empty()) {
        return true;
    }
    position curr_pos = _tokens.position();
    std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
    std::cout << "Error: no more one function\n";
    return false;
}

bool parser::begin() {
    position curr_pos = _tokens.position();
    token curr = _tokens.get_next_token();
    if (curr.type() != INT && curr.type() != CHAR) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in the type of function: the type should be int or char\n";
        return false;
    }
    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.type() != ID) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in the function name: the name must be the correct identifier\n";
        return false;
    }
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

bool parser::description() {
    if (! small_descriptions()) {
        return false;
    }
    while (true) {
        token curr = _tokens.get_next_token();
        if (curr.type() != INT && curr.type() != CHAR) {
            _tokens.return_last_word();
            return true;
        }
        _tokens.return_last_word();
        if (! small_descriptions()) {
            return false;
        }
    }
}

bool parser::small_descriptions() {
    position curr_pos = _tokens.position();
    token curr = _tokens.get_next_token();
    if (curr.type() != INT && curr.type() != CHAR) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in the description: "
                  << "the description should start with the type of variable\n";
    }
    if (! var_list()) {
        return false;
    }
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

bool parser::var_list() {
    position curr_pos = _tokens.position();
    token curr = _tokens.get_next_token();
    if (curr.type() != ID) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in the description: no identifier in description\n";
        return false;
    }
    curr = _tokens.get_next_token();
    if (curr.type() != COMMA) {
        _tokens.return_last_word();
        return true;
    }
    return var_list();
}

bool parser::operators() {
    if (! small_operators()) {
        return false;
    }
    while (true) {
        token curr = _tokens.get_next_token();
        if (curr.type() != ID) {
            _tokens.return_last_word();
            return true;
        }
        _tokens.return_last_word();
        if (! small_operators()) {
            return false;
        }
    }
}

bool parser::small_operators() {
    position curr_pos = _tokens.position();
    token curr = _tokens.get_next_token();
    if (curr.type() != ID) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in operators: expression must be assigned to the variable\n";
        return false;
    }
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
        if (! num_expr()) {
            return false;
        }
    } else if (curr.type() == SIMPLE_STRING) {
        _tokens.return_last_word();
        if (! string_expr()) {
            return false;
        }
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

bool parser::num_expr() {
    if (! simple_num_expr()) {
        return false;
    }
    token curr = _tokens.get_next_token();
    if (curr.type() != SUM && curr.type() != MINUS) {
        _tokens.return_last_word();
        return true;
    }
    return num_expr();
}

bool parser::simple_num_expr() {
    position curr_pos = _tokens.position();
    token curr = _tokens.get_next_token();
    if (curr.type() == LBRACKET) {
        if (! num_expr()) {
            return false;
        }
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
    return true;
}

bool parser::string_expr() {
    token curr = _tokens.get_next_token();
    if (curr.type() != SIMPLE_STRING) {
        return false;
    }
    curr = _tokens.get_next_token();
    if (curr.type() != SUM) {
        _tokens.return_last_word();
        return true;
    }
    return string_expr();
}

bool parser::end() {
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
