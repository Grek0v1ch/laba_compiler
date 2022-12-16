#pragma once

#include "parse_tree.h"

#include <string>

class semantic {
private:
    parse_tree _tree;

private:
    void feel_begin();
    void feel_end();
    void feel_description(std::shared_ptr<tree_node>& curr_description);
    void feel_descr(std::shared_ptr<tree_node>& curr_descr);
    int feel_var_list(std::shared_ptr<tree_node>& var_list);
    void feel_operators(std::shared_ptr<tree_node>& curr_operators);
    void feel_op(std::shared_ptr<tree_node>& curr_op);
    void feel_expr(std::shared_ptr<tree_node>& curr_expr);
    void feel_simple_expr(std::shared_ptr<tree_node>& curr_simple_expr);

public:
    semantic(const parse_tree& v) : _tree{v} {}
    void run();
    std::string get_rpn() const { return _tree._root->_rpn; }
};
