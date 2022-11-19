#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include "lexical_item.h"
#include "token.h"
#include "terminal.h"
#include <vector>
#include <memory>

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

    node(const node& v) {
        _children = std::vector<std::shared_ptr<node>>(v._children);
        if (v._value == nullptr) {
            _value = nullptr;
            return;
        }
        if (v._value->class_name() == "terminal") {
            _value = std::dynamic_pointer_cast<terminal>(v._value);
        }
        if (v._value->class_name() == "token") {
            _value = std::dynamic_pointer_cast<token>(v._value);
        }
    }

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
    ~parse_tree() = default;
    void print(std::ostream& out) { print(out,_root, 0); }
    void add_token(const terminal& to_add, const token& v) { add_token(to_add, v, _root); }
    void add_product(const terminal& to_add, const type_product& product) { add_product(to_add, product, _root); }
private:
    pnode _root;

    void print (std::ostream& out, const pnode& pos, size_t level) {
        // Не нужно отслеживать pos == nullptr так как корень всегда инициализирован, а если у
        // какого-то элемента дерева нет детей, то мы просто не заходим в цикл по его детям
        for (size_t i = 0; i < level; i++) {
            out << '\t';
        }
        if (pos->_value->class_name() == "terminal") {
            terminal temp = *std::dynamic_pointer_cast<terminal>(pos->_value);
            out << temp << '\n';
        } else if (pos->_value->class_name() == "token") {
            token temp = *std::dynamic_pointer_cast<token>(pos->_value);
            out << temp << '\n';
        }
        for (auto& item : pos->_children) {
            print(out, item, level + 1);
        }
    }

    // Метод добавления токена в дерево
    bool add_token(const terminal& to_add, const token& v, pnode& pos) {
        if (! pos) {
            return false;
        }
        if (pos->_value->class_name() == "terminal") {
            terminal temp = *std::dynamic_pointer_cast<terminal>(pos->_value);
            if (temp.type() == to_add.type()) {
                pos->_children.push_back(std::make_shared<node>(v));
                return true;
            }
        }
        for (auto& item : pos->_children) {
            if (add_token(to_add, v, item)) {
                return true;
            }
        }
        return false;
    }

    bool add_product(const terminal& to_add, type_product product, pnode& pos) {
        if (! pos) {
            return false;
        }
        if (pos->_value->class_name() == "terminal") {
            terminal temp = *std::dynamic_pointer_cast<terminal>(pos->_value);
            if (temp.type() == to_add.type()) {
                push_product_item(pos->_children, product);
                return true;
            }
        }
        for (auto& item : pos->_children) {
            if (add_product(to_add, product, item)) {
                return true;
            }
        }
        return false;
    }

    void push_product_item(std::vector<std::shared_ptr<node>>& children, type_product product) {
        if (product == FUNCTION) {
            children.push_back(std::make_shared<node>(terminal{terminal::BEGIN}));
            children.push_back(std::make_shared<node>(terminal{terminal::DESCRIPTION}));
            children.push_back(std::make_shared<node>(terminal{terminal::OPERATORS}));
            children.push_back(std::make_shared<node>(terminal{terminal::END}));
        } else if (product == BEGIN) {
            children.push_back(std::make_shared<node>(terminal{terminal::TYPE}));
            children.push_back(std::make_shared<node>(terminal{terminal::FUNCTION_NAME}));
            children.push_back(std::make_shared<node>(token{LBRACKET, "("}));
            children.push_back(std::make_shared<node>(token{RBRACKET, ")"}));
            children.push_back(std::make_shared<node>(token{LBRACKET_FIGURE, "{"}));
        } else if (product == END) {
            children.push_back(std::make_shared<node>(token{RETURN, "return"}));
            children.push_back(std::make_shared<node>(terminal{terminal::ID}));
            children.push_back(std::make_shared<node>(token{SEMICOLON, ";"}));
            children.push_back(std::make_shared<node>(token{RBRACKET_FIGURE, "}"}));
        } else if (product == FUNCTION_NAME) {
            children.push_back(std::make_shared<node>(terminal{terminal::ID}));
        } else if (product == DESCRIPTION_1) {
            children.push_back(std::make_shared<node>(terminal{terminal::DESCR}));
        } else if (product == DESCRIPTION_2) {
            children.push_back(std::make_shared<node>(terminal{terminal::DESCR}));
            children.push_back(std::make_shared<node>(terminal{terminal::DESCRIPTION}));
        } else if (product == DESCR) {
            children.push_back(std::make_shared<node>(terminal{terminal::TYPE}));
            children.push_back(std::make_shared<node>(terminal{terminal::VAR_LIST}));
            children.push_back(std::make_shared<node>(token{SEMICOLON, ";"}));
        } else if (product == TYPE_1) {
            children.push_back(std::make_shared<node>(token{INT, "int"}));
        } else if (product == TYPE_2) {
            children.push_back(std::make_shared<node>(token{CHAR, "char"}));
        } else if (product == VAR_LIST_1) {
            children.push_back(std::make_shared<node>(terminal{terminal::ID}));
        } else if (product == VAR_LIST_2) {
            children.push_back(std::make_shared<node>(terminal{terminal::ID}));
            children.push_back(std::make_shared<node>(token{COMMA, ","}));
            children.push_back(std::make_shared<node>(terminal{terminal::VAR_LIST}));
        } else if (product == OPERATORS_1) {
            children.push_back(std::make_shared<node>(terminal{terminal::OP}));
        } else if (product == OPERATORS_2) {
            children.push_back(std::make_shared<node>(terminal{terminal::OP}));
            children.push_back(std::make_shared<node>(terminal{terminal::OPERATORS}));
        } else if (product == OP_1) {
            children.push_back(std::make_shared<node>(terminal{terminal::ID}));
            children.push_back(std::make_shared<node>(token{EQUALS, "="}));
            children.push_back(std::make_shared<node>(terminal{terminal::NUM_EXPR}));
            children.push_back(std::make_shared<node>(token{SEMICOLON, ";"}));
        } else if (product == OP_2) {
            children.push_back(std::make_shared<node>(terminal{terminal::ID}));
            children.push_back(std::make_shared<node>(token{EQUALS, "="}));
            children.push_back(std::make_shared<node>(terminal{terminal::STRING_EXPR}));
            children.push_back(std::make_shared<node>(token{SEMICOLON, ";"}));
        } else if (product == NUM_EXPR_1) {
            children.push_back(std::make_shared<node>(terminal{terminal::SIMPLE_NUM_EXPR}));
        } else if (product == NUM_EXPR_2) {
            children.push_back(std::make_shared<node>(terminal{terminal::SIMPLE_NUM_EXPR}));
            children.push_back(std::make_shared<node>(token{SUM, "+"}));
            children.push_back(std::make_shared<node>(terminal{terminal::NUM_EXPR}));
        } else if (product == NUM_EXPR_3) {
            children.push_back(std::make_shared<node>(terminal{terminal::SIMPLE_NUM_EXPR}));
            children.push_back(std::make_shared<node>(token{MINUS, "-"}));
            children.push_back(std::make_shared<node>(terminal{terminal::NUM_EXPR}));
        } else if (product == SIMPLE_NUM_EXPR_1) {
            children.push_back(std::make_shared<node>(terminal{terminal::ID}));
        } else if (product == SIMPLE_NUM_EXPR_2) {
            children.push_back(std::make_shared<node>(terminal{terminal::CONST}));
        } else if (product == SIMPLE_NUM_EXPR_3) {
            children.push_back(std::make_shared<node>(token{LBRACKET, "("}));
            children.push_back(std::make_shared<node>(terminal{terminal::NUM_EXPR}));
            children.push_back(std::make_shared<node>(token{RBRACKET, ")"}));
        } else if (product == STRING_EXPR_1) {
            children.push_back(std::make_shared<node>(terminal{terminal::SIMPLE_STRING_EXPR}));
        } else if (product == STRING_EXPR_2) {
            children.push_back(std::make_shared<node>(terminal{terminal::SIMPLE_STRING_EXPR}));
            children.push_back(std::make_shared<node>(token{SUM, "+"}));
            children.push_back(std::make_shared<node>(terminal{terminal::STRING_EXPR}));
        }
    }
};


#endif // PARSE_TREE_H
