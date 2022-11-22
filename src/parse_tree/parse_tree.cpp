#include "parse_tree.h"
#include <memory>

tree_node::tree_node(const tree_node& v) {
    _children = std::vector<std::shared_ptr<tree_node>>(v._children);
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

void parse_tree::print (std::ostream& out, const ptree_node& pos, size_t level) {
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

void parse_tree::insert_tree(const terminal& to_add, const parse_tree& tree, const ptree_node& pos) {
    if (! pos) {
        return;
    }
    for (auto& item : pos->_children) {
        if (item->_value->class_name() == "terminal" && item->_children.empty()) {
            terminal temp = *std::dynamic_pointer_cast<terminal>(item->_value);
            if (temp.type() == to_add.type()) {
                item = nullptr;
                copy_tree(item, tree._root);
                return;
            }
        }
        insert_tree(to_add, tree, item);
    }
}

void parse_tree::add_tree(const terminal& to_add, const parse_tree& tree, const ptree_node& pos) {
    if (! pos) {
        return;
    }
    if (pos->_value->class_name() == "terminal" && pos->_children.empty()) {
        terminal temp = *std::dynamic_pointer_cast<terminal>(pos->_value);
        if (temp.type() == to_add.type()) {
            pos->_children.push_back(std::make_shared<tree_node>());
            copy_tree(*pos->_children.begin(), tree._root);
            return;
        }
    }
    for (const auto& item : pos->_children) {
        add_tree(to_add, tree, item);
    }
}

void parse_tree::copy_tree(ptree_node& to, ptree_node from) {
    to = std::make_shared<tree_node>(*from);
    for (size_t i = 0; i < from->_children.size(); i++) {
        copy_tree(to->_children[i], from->_children[i]);
    }
}

// Метод добавления токена в дерево
bool parse_tree::add_token(const terminal& to_add, const token& v, ptree_node& pos) {
    if (! pos) {
        return false;
    }
    if (pos->_value->class_name() == "terminal" && pos->_children.empty()) {
        terminal temp = *std::dynamic_pointer_cast<terminal>(pos->_value);
        if (temp.type() == to_add.type()) {
            pos->_children.push_back(std::make_shared<tree_node>(v));
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

bool parse_tree::add_product(const terminal& to_add, type_product product, ptree_node& pos) {
    if (! pos) {
        return false;
    }
    if (pos->_value->class_name() == "terminal" && pos->_children.empty()) {
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

void parse_tree::push_product_item(std::vector<std::shared_ptr<tree_node>>& children, type_product product) {
    if (product == FUNCTION) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::BEGIN}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::DESCRIPTION}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::OPERATORS}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::END}));
    } else if (product == BEGIN) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::TYPE}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::FUNCTION_NAME}));
        children.push_back(std::make_shared<tree_node>(token{LBRACKET, "("}));
        children.push_back(std::make_shared<tree_node>(token{RBRACKET, ")"}));
        children.push_back(std::make_shared<tree_node>(token{LBRACKET_FIGURE, "{"}));
    } else if (product == END) {
        children.push_back(std::make_shared<tree_node>(token{RETURN, "return"}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::ID}));
        children.push_back(std::make_shared<tree_node>(token{SEMICOLON, ";"}));
        children.push_back(std::make_shared<tree_node>(token{RBRACKET_FIGURE, "}"}));
    } else if (product == FUNCTION_NAME) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::ID}));
    } else if (product == DESCRIPTION_1) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::DESCR}));
    } else if (product == DESCRIPTION_2) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::DESCR}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::DESCRIPTION}));
    } else if (product == DESCR) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::TYPE}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::VAR_LIST}));
        children.push_back(std::make_shared<tree_node>(token{SEMICOLON, ";"}));
    } else if (product == TYPE_1) {
        children.push_back(std::make_shared<tree_node>(token{INT, "int"}));
    } else if (product == TYPE_2) {
        children.push_back(std::make_shared<tree_node>(token{CHAR, "char"}));
    } else if (product == VAR_LIST_1) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::ID}));
    } else if (product == VAR_LIST_2) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::ID}));
        children.push_back(std::make_shared<tree_node>(token{COMMA, ","}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::VAR_LIST}));
    } else if (product == OPERATORS_1) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::OP}));
    } else if (product == OPERATORS_2) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::OP}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::OPERATORS}));
    } else if (product == OP_1) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::ID}));
        children.push_back(std::make_shared<tree_node>(token{EQUALS, "="}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::NUM_EXPR}));
        children.push_back(std::make_shared<tree_node>(token{SEMICOLON, ";"}));
    } else if (product == OP_2) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::ID}));
        children.push_back(std::make_shared<tree_node>(token{EQUALS, "="}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::STRING_EXPR}));
        children.push_back(std::make_shared<tree_node>(token{SEMICOLON, ";"}));
    } else if (product == NUM_EXPR_1) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::SIMPLE_NUM_EXPR}));
    } else if (product == NUM_EXPR_2) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::SIMPLE_NUM_EXPR}));
        children.push_back(std::make_shared<tree_node>(token{SUM, "+"}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::NUM_EXPR}));
    } else if (product == NUM_EXPR_3) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::SIMPLE_NUM_EXPR}));
        children.push_back(std::make_shared<tree_node>(token{MINUS, "-"}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::NUM_EXPR}));
    } else if (product == SIMPLE_NUM_EXPR_1) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::ID}));
    } else if (product == SIMPLE_NUM_EXPR_2) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::CONST}));
    } else if (product == SIMPLE_NUM_EXPR_3) {
        children.push_back(std::make_shared<tree_node>(token{LBRACKET, "("}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::NUM_EXPR}));
        children.push_back(std::make_shared<tree_node>(token{RBRACKET, ")"}));
    } else if (product == STRING_EXPR_1) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::SIMPLE_STRING_EXPR}));
    } else if (product == STRING_EXPR_2) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::SIMPLE_STRING_EXPR}));
        children.push_back(std::make_shared<tree_node>(token{SUM, "+"}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::STRING_EXPR}));
    }
}
