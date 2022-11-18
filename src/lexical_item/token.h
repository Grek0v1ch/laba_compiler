#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>
#include "lexical_item.h"

// Типы лексем
enum type_lexeme {
    UNKNOWN,
    RETURN, INT, CHAR,
    ID, NUMBER, SIMPLE_STRING,
    SUM, MINUS,
    LBRACKET, RBRACKET,
    EQUALS, SEMICOLON, LBRACKET_FIGURE, RBRACKET_FIGURE, COMMA,
};

// Класс токена. Токен - это пара (тип_лексемы, лексема)
class token : public lexical_item {
private:
    type_lexeme _type;
    std::string _text;

public:
    token() : _type(UNKNOWN), _text() {}
    token(const type_lexeme& type, std::string  text) : _type(type), _text(std::move(text)) {}
    token(const type_lexeme& type, char text);
    token(const token& v) = default;

    std::string text() const { return _text; }
    type_lexeme type() const { return _type; }

    void print(std::ostream& out) const override;

    bool operator==(const token& v) const&;
    token& operator=(const token& v);
    friend std::ostream& operator<<(std::ostream& out, const token& v);
};


#endif // TOKEN_H
