#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>
#include "lexical_item.h"

/**
 * Класс токена. Наследуется от интерфейса lexical_item. Содержит в себе тип лексемы и ее текстовое
 * представление
 * \param _type - тип лексемы
 * \param _text - текстовое представление лексемы
 * \param _var_type - тип переменной (только для токенов переменных, для остальных не используется)
 * */
class token : public lexical_item {
public:
    // Типы лексем
    enum type_lexeme {
        UNKNOWN,
        RETURN, INT, CHAR,
        ID, NUMBER, SIMPLE_STRING,
        SUM, MINUS,
        LBRACKET, RBRACKET,
        EQUALS, SEMICOLON, LBRACKET_FIGURE, RBRACKET_FIGURE, COMMA,
    };

    /**
     * Конструктор по умолчанию. Токен инициализируется как {UNKNOWN, "", ""}
     * */
    token() : _type(UNKNOWN), _text(), _var_type() {}
    /**
     * Конструктор по типу токена и его текстовому представлению
     * \param type - тип токена
     * \param text - его текстовое представление
     * */
    token(const type_lexeme& type, std::string  text) : _type(type), _text(std::move(text)), _var_type() {}
    /**
     * Конструктор по типу токена и его текстовому представлению
     * \param type - тип токена
     * \param text - его текстовое представление
     * */
    token(const type_lexeme& type, char text);
    /**
     * Дефолтный конструктор копирования
     * */
    token(const token& v) = default;

    /**
     * Метод возвращает текстовое представление токена
     * \return текстовое представление токена
     * */
    std::string text() const { return _text; }
    /**
     * Метод возвращает тип токена
     * \return тип токена
     * */
    type_lexeme type() const { return _type; }
    /**
     * Метод возвращает тип переменной
     * \return тип переменной
     * */
    std::string var_type() const { return _var_type; }

    /**
     * Метод изменяет значение поля _var_type
     * \param var_type новое значение поля
     * */
    void set_var_type(std::string& var_type) { _var_type = var_type; }

    /**
     * Реализация метода из интерфейса lexical_item
     * Метод возвращает имя класса
     * \return "token"
     * */
    std::string class_name() const override { return {"token"}; }

    bool operator==(const token& v) const&;
    token& operator=(const token& v);
    friend std::ostream& operator<<(std::ostream& out, const token& v);

private:
    type_lexeme _type;
    std::string _text;
    // Только для переменных. Для остальных токенов он пуст
    std::string _var_type;
};


#endif // TOKEN_H
