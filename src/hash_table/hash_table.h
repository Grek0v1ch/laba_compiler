#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <string>
#include <vector>
#include <iostream>
#include "token.h"

// Класс хеш-таблицы.
// В качестве второго передаваемого класса следует указатель класс-обертку над хеш-функцией.
// В классе обертке необходима переадресация оператора ().

class node {
private:
    token _key;
    bool _is_del;
public:
    node() : _key(token()), _is_del(false) {}
    node(token& key, bool is_del) : _key(key), _is_del(is_del) {}
    friend class hash_table;
};

using pnode = node*;

class hash_table {
private:
    const size_t DEFAULT_SIZE = 7;
    const double REHASH = 0.75;
    size_t _size;
    std::vector<pnode> _arr;

    void rehash();
    size_t hash_function(const std::string&, size_t);

public:
    hash_table() : _size(0), _arr(std::vector<pnode>(DEFAULT_SIZE, nullptr)) {}
    hash_table(const hash_table& v);
    ~hash_table();

    size_t size() const { return _size; }

    void add(token value);
    void remove(token value);

    type_lexeme find_lexeme(std::string& text);

    void print();
    std::vector<std::pair<size_t, token>> to_array() const;
};

#endif // HASH_TABLE_H
