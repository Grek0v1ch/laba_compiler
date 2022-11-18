#include "token.h"

void token::print(std::ostream& out) const {
    out << *this;
}

token::token(const type_lexeme& type, const char text) {
    _type = type;
    _text = std::string();
    _text += text;
}

bool token::operator==(const token& v) const& {
    return _type == v._type && _text == v._text;
}

token& token::operator=(const token& v) {
    if (&v == this) {
        return *this;
    }
    _type = v._type;
    _text = v._text;
    return *this;
}

std::ostream& operator<<(std::ostream& out, const token& v) {
    std::string output;
    switch (v._type) {
        case RETURN:
            output = "Return";
            break;
        case INT:
            output = "Keyword Int";
            break;
        case CHAR:
            output = "Keyword Char";
            break;
        case ID:
            output = "Id";
            break;
        case NUMBER:
            output = "Number";
            break;
        case SIMPLE_STRING:
            output = "Simple String";
            break;
        case SUM:
            output = "Sum";
            break;
        case MINUS:
            output = "Minus";
            break;
        case LBRACKET:
            output = "Left Bracket";
            break;
        case RBRACKET:
            output = "Right Bracket";
            break;
        case EQUALS:
            output = "Equals";
            break;
        case SEMICOLON:
            output = "Semicolon";
            break;
        case LBRACKET_FIGURE:
            output = "Left Bracket Figure";
            break;
        case RBRACKET_FIGURE:
            output = "Right Bracket Figure";
            break;
        case COMMA:
            output = "Comma";
            break;
        case UNKNOWN:
            output = "Unknown";
            break;
        default:
            break;
    }
    out << output << ' ' << v._text;
    return out;
}
