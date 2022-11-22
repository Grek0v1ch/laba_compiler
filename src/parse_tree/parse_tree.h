#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include "lexical_item.h"
#include "token.h"
#include "terminal.h"
#include <vector>

using ptr_lexi = std::shared_ptr<lexical_item>;
class parse_tree;

class node {
public:
    node() : _value(nullptr), _children(std::vector<std::shared_ptr<node>>()) {}

    node(const terminal& v) :
        _value(std::static_pointer_cast<lexical_item>(std::make_shared<terminal>(v))),
        _children(std::vector<std::shared_ptr<node>>()) {}

    node(const token& v) :
        _value(std::static_pointer_cast<lexical_item>(std::make_shared<token>(v))),
        _children(std::vector<std::shared_ptr<node>>()) {}

    node(const node& v);

    ~node() = default;

    friend class parse_tree;

private:
    ptr_lexi _value;
    std::vector<std::shared_ptr<node>> _children;
};

using pnode = std::shared_ptr<node>;

class parse_tree {
public:
    enum type_product {
        FUNCTION,
        BEGIN,
        END,
        FUNCTION_NAME,
        DESCRIPTION_1, DESCRIPTION_2,
        DESCR,
        TYPE_1, TYPE_2,
        VAR_LIST_1, VAR_LIST_2,
        OPERATORS_1, OPERATORS_2,
        OP_1, OP_2,
        NUM_EXPR_1, NUM_EXPR_2, NUM_EXPR_3,
        SIMPLE_NUM_EXPR_1, SIMPLE_NUM_EXPR_2, SIMPLE_NUM_EXPR_3,
        STRING_EXPR_1, STRING_EXPR_2,
        SIMPLE_STRING_EXPR
    };

    parse_tree() : _root(std::make_shared<node>(node{terminal{terminal::FUNCTION}})) {}
    parse_tree(const terminal& root) : _root(std::make_shared<node>(node{root})) {}
    ~parse_tree() = default;
    void print(std::ostream& out) { print(out,_root, 0); }
    void add_tree(const terminal& to_add, const parse_tree& tree) {
        add_tree(to_add, tree, _root); }
    void add_token(const terminal& to_add, const token& v) {
        add_token(to_add, v, _root); }
    void add_product(const terminal& to_add, const type_product& product) {
        add_product(to_add, product, _root); }
private:
    pnode _root;

    void print (std::ostream& out, const pnode& pos, size_t level);
    void add_tree(const terminal& to_add, const parse_tree& tree, const pnode& pos);
    void copy_tree(pnode& to, pnode from);
    // Метод добавления токена в дерево
    bool add_token(const terminal& to_add, const token& v, pnode& pos);
    bool add_product(const terminal& to_add, type_product product, pnode& pos);
    void push_product_item(std::vector<std::shared_ptr<node>>& children, type_product product);
};


#endif // PARSE_TREE_H
