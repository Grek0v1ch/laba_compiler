#ifndef TERMINAL_H
#define TERMINAL_H

#include "lexical_item.h"

class terminal : public lexical_item {
public:
    enum type_terminal {
        FUNCTION, BEGIN, END, FUNCTION_NAME,
        DESCRIPTION, DESCR,
        TYPE, VAR_LIST,
        OPERATORS, OP,
        NUM_EXPR, SIMPLE_NUM_EXPR,
        STRING_EXPR, SIMPLE_STRING_EXPR
    };
    terminal() : _type(FUNCTION) {}
    terminal(type_terminal type) : _type(type) {}

    friend std::ostream& operator<<(std::ostream& out, const terminal& v);

private:
    type_terminal _type;
};

std::ostream& operator<<(std::ostream& out, const terminal& v) {

}

#endif // TERMINAL_H
