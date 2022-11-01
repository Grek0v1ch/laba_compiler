#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <string>
#include <vector>
#include <iostream>

// Класс хеш-таблицы.
// В качестве второго передаваемого класса следует указатель класс-обертку над хеш-функцией.
// В классе обертке необходима переадресация оператора ().
template <class T, class T_hash>
class hash_table;

template <class T>
class node {
private:
    T _key;
    bool _is_del;
public:
    node() : _key(T()), _is_del(false) {}
    node(T key, bool is_del) : _key(key), _is_del(is_del) {}
    template<class T1, class T_hash> friend class hash_table;
};

template <typename Type> using pnode = node<Type>*;

template <class T, class T_hash>
class hash_table {
private:
    const size_t DEFAULT_SIZE = 7;
    const double REHASH = 0.75;
    size_t _size;
    std::vector<pnode<T>> _arr;

    void rehash() {
        _size = 0;
        std::vector<pnode<T>> arr(_arr);
        std::fill(_arr.begin(),  _arr.end(), nullptr);
        _arr.resize(_arr.size() * 2, 0);
        for (size_t i = 0; i < arr.size(); i++) {
            if (arr[i] != 0 && ! arr[i]->_is_del) {
                add(arr[i]->_key);
            }
        }
    }

public:
    hash_table() : _size(0), _arr(std::vector<pnode<T>>(DEFAULT_SIZE, 0)) {}
    ~hash_table() {
        for (size_t i = 0; i < _arr.size(); i++) {
            delete _arr[i];
        }
        _arr.clear();
    }

    size_t size() const { return _size; }

    void add(T value) {
        if (REHASH <= (_size * 1.0 / _arr.size())) {
            rehash();
        }
        T_hash hash = T_hash();
        size_t idx = hash(value, _arr.size());
        while (_arr[idx] != 0 && ! _arr[idx]->_is_del) {
            if (_arr[idx]->_key == value) {
                return;
            }
            idx = (idx + 1) % _arr.size();
        }
        _size++;
        if (_arr[idx] == 0) {
            _arr[idx] = new node<T>(value, false);
        } else {
            _arr[idx]->_key = value;
            _arr[idx]->_is_del = false;
        }
    }

    void remove(T value) {
        T_hash hash = T_hash();
        size_t idx = hash(value, _arr.size());
        while (_arr[idx] != 0) {
            if (_arr[idx]->_key == value) {
                _arr[idx]->_key = T();
                _arr[idx]->_is_del = true;
                _size--;
                return;
            }
            idx++;
        }
    }

    void print() {
        for (int i = 0; i < _arr.size(); i++) {
            if (_arr[i] == 0 || _arr[i]->_is_del) {
                continue;
            }
            std::cout << "Key: " << i << "\tvalue: " << _arr[i]->_key << '\n';
        }
    }

    std::vector<std::pair<size_t, T>> to_array() const {
        std::vector<std::pair<size_t, T>> res;
        for (size_t i = 0; i < _arr.size(); i++) {
            if (_arr[i] == 0 || _arr[i]->_is_del) {
                continue;
            }
            res.push_back(std::make_pair(i, _arr[i]->_key));
        }
        return res;
    }
};

#endif // HASH_TABLE_H
