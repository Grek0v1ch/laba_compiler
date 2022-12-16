#include "token.h"

token::token(const type_lexeme& type, const char text) {
    _type = type;
    _text = std::string();
    _text += text;
    _var_type = std::string();
}

bool token::operator==(const token& v) const& {
    return _type == v._type && _text == v._text && _var_type == v._var_type;
}

token& token::operator=(const token& v) {
    if (&v == this) {
        return *this;
    }
    _type = v._type;
    _text = v._text;
    _var_type = v._var_type;
    return *this;
}

std::ostream& operator<<(std::ostream& out, const token& v) {
    std::string output;
    switch (v._type) {
        case token::RETURN:
            output = "Keyword Return";
            break;
        case token::INT:
            output = "Keyword Int";
            break;
        case token::CHAR:
            output = "Keyword Char";
            break;
        case token::ID:
            output = "Id";
            break;
        case token::NUMBER:
            output = "Number";
            break;
        case token::SIMPLE_STRING:
            output = "Simple String";
            break;
        case token::SUM:
            output = "Sum";
            break;
        case token::MINUS:
            output = "Minus";
            break;
        case token::LBRACKET:
            output = "Left Bracket";
            break;
        case token::RBRACKET:
            output = "Right Bracket";
            break;
        case token::EQUALS:
            output = "Equals";
            break;
        case token::SEMICOLON:
            output = "Semicolon";
            break;
        case token::LBRACKET_FIGURE:
            output = "Left Bracket Figure";
            break;
        case token::RBRACKET_FIGURE:
            output = "Right Bracket Figure";
            break;
        case token::COMMA:
            output = "Comma";
            break;
        case token::UNKNOWN:
            output = "Unknown";
            break;
        default:
            break;
    }
    out << output << ' ' << v._text;
    if (! v._var_type.empty()) {
        out << " Var_type:" << v._var_type;
    }
    return out;
}
