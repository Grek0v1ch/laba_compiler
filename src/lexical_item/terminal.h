#ifndef TERMINAL_H
#define TERMINAL_H

#include <iostream>
#include <string>
#include "lexical_item.h"

class terminal : public lexical_item {
public:
    enum type_terminal {
        FUNCTION, BEGIN, END, FUNCTION_NAME,
        DESCRIPTION, DESCR,
        TYPE, VAR_LIST,
        OPERATORS, OP,
        NUM_EXPR, SIMPLE_NUM_EXPR,
        STRING_EXPR, SIMPLE_STRING_EXPR,
        ID, CONST
    };
    terminal() : _type(FUNCTION) {}
    terminal(const terminal& v) : _type(v._type) {}
    terminal(type_terminal type) : _type(type) {}

    std::string class_name() const override { return {"terminal"}; }
    type_terminal type() const { return _type; }

    friend std::ostream& operator<<(std::ostream& out, const terminal& v);

private:
    type_terminal _type;
};

#endif // TERMINAL_H
