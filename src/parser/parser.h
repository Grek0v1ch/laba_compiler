#ifndef PARSER_H
#define PARSER_H

#include "lexical_analyzer.h"
#include "token.h"
#include "parse_tree.h"

class parser {
private:
    lexical_analyzer _tokens;
    parse_tree _parse_tree;

    void check_lexical_error(token& curr);
    bool begin(parse_tree& begin_tree);
    bool description(parse_tree& description_tree);
    bool small_descriptions(parse_tree& small_description_tree);
    bool var_list(parse_tree& var_list_tree);
    bool operators(parse_tree& operators_tree);
    bool small_operators(parse_tree& small_operators_tree);
    bool num_expr(parse_tree& num_expr_tree);
    bool simple_num_expr(parse_tree& simple_num_expr_tree);
    bool string_expr(parse_tree& string_expr_tree);
    bool end(parse_tree& end_tree);

public:
    void print_parse_tree(std::ostream& out) { _parse_tree.print(out); }
    parse_tree get_parse_tree() const { return _parse_tree; }
    hash_table get_hash_table() { return _tokens.tokens(); }
    parser(const char* file_name) : _tokens(file_name), _parse_tree() {}
    bool run();
};


#endif // PARSER_H
