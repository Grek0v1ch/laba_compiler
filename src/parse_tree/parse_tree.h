#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include "lexical_item.h"
#include "token.h"
#include "terminal.h"
#include <vector>

using ptr_lexi = lexical_item*;
class parse_tree;

class node {
public:
    node() : _value(nullptr), _children(std::vector<node*>()) {}

    node(const lexical_item& value) {
        _value = new lexical_item;
        *_value = value;
        _children = std::vector<node*>();
    }

    node(const node& v) {
        _value = new lexical_item;
        *_value = *v._value;
        _children = std::vector<node*>(v._children);
    }

    ~node() {
        delete _value;
        for (auto& item : _children) {
            delete item;
        }
    }

    friend class parse_tree;

private:
    ptr_lexi _value;
    std::vector<node*> _children;
};

using pnode = node*;

class parse_tree {
public:
    enum product {
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

    parse_tree() : _root(nullptr) {}
    ~parse_tree() { destroy(_root); }
    void print(std::ostream& out) { print(out,_root, 0); }
private:
    pnode _root;

    void print (std::ostream& out, pnode pos, size_t level) {
        if (! pos) {
            return;
        }
        for (size_t i = 0; i < level; i++) {
            out << '\t';
        }
        out << *(pos->_value) << '\n';
        for (auto& item : pos->_children) {
            print(out, item, level + 1);
        }
    }

    void destroy(pnode& pos) {
        if (! pos) {
            return;
        }
        for (auto& item : pos->_children) {
            destroy(item);
        }
        delete pos;
    }
};


#endif // PARSE_TREE_H
