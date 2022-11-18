#ifndef PARSER_H
#define PARSER_H

#include "lexical_analyzer.h"
#include "token.h"

class parser {
private:
    lexical_analyzer _tokens;

    void check_lexical_error(token& curr);
    bool begin();
    bool description();
    bool small_descriptions();
    bool var_list();
    bool operators();
    bool small_operators();
    bool num_expr();
    bool simple_num_expr();
    bool string_expr();
    bool end();

public:
    parser(const char* file_name) : _tokens(file_name) {}
    bool run();
};


#endif // PARSER_H
