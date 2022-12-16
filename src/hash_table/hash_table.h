#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <string>
#include <vector>
#include <iostream>
#include "token.h"

/**
 * Класс узла хеш таблицы.
 * Содержит в себе непосредственно значение узла и метку, что узел был удален
 */
class node {
private:
    token _key;
    bool _is_del;
public:
    /**
     * Конструктор по умолчанию
     * */
    node() : _key(token()), _is_del(false) {}
    /**
     * Конструктор по ключу и метке
     * \param key - ключ узла
     * \param is_del - метка, удален ли узел
     * */
    node(token& key, bool is_del) : _key(key), _is_del(is_del) {}
    // Класс хеш таблицы объявлен дружественным, чтобы иметь доступ к его полям
    friend class hash_table;
};

using pnode = node*;

/**
 * Класс хеш таблицы. Хеш таблица реализована через метод открытой адресации с методом линейного
 * пробирования в качестве метода разрешения коллизий
 * */
class hash_table {
private:
    const size_t DEFAULT_SIZE = 7;
    // Коэффициент заполнения таблицы, при котором необходимо сделать расширение
    const double REHASH = 0.75;
    // Переменная содержит размер хеш таблицы - число не удаленных узлов.
    size_t _size;
    // Размер же выделенной памяти под таблицу можно получить через размер поля _arr.
    std::vector<pnode> _arr;

    /**
     * Метод ре-хеширования (увеличения размера выделенной памяти)
     * */
    void rehash();
    /**
     * Хеш-функция для строк.
     * Так как хеш-таблица хранит токены, а у токенов есть текстовое представление, то по этому
     текстовому представлению и получаем хеш
     */
    size_t hash_function(const std::string&, size_t);

public:
    /**
     * Конструктор по умолчанию
     * */
    hash_table() : _size(0), _arr(std::vector<pnode>(DEFAULT_SIZE, nullptr)) {}
    /**
     * Конструктор копирования
     * \param v - хеш-таблица для копирования
     * */
    hash_table(const hash_table& v);
    ~hash_table();

    /**
     * Метод возвращает размер хеш-таблицы
     * */
    size_t size() const { return _size; }

    void add(token value);
    void remove(token value);

    void set_type(std::string& text, std::string& var_type);
    token find_lexeme(std::string& text);

    // Метод печати хеш-таблицы в консоль
    void print();

    // Метод возвращает токен с данным текстовым представлением
    std::vector<std::pair<size_t, token>> to_array() const;
};

#endif // HASH_TABLE_H
