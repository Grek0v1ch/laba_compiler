#include "hash_table.h"

void hash_table::rehash() {
    _size = 0;
    std::vector<pnode> arr(_arr);
    std::fill(_arr.begin(),  _arr.end(), nullptr);
    _arr.resize(_arr.size() * 2, nullptr);
    for (auto & item : arr) {
        if (item != nullptr && ! item->_is_del) {
            add(item->_key);
        }
    }
    for (auto & item : arr) {
        delete item;
    }
}

size_t hash_table::hash_function(const std::string& s, const size_t table_size) {
    size_t hash_result = 0;
    const size_t hash_coef = 57;
    for (char i : s) {
        hash_result = (hash_coef * hash_result + i) % table_size;
    }
    hash_result = (hash_result * 2 + 1) % table_size;
    return hash_result;
}

hash_table::hash_table(const hash_table& v) : _size(v._size) {
    _arr = std::vector<pnode>(v._arr.size());
    for (size_t i = 0; i < v._arr.size(); i++) {
        if (v._arr[i] == nullptr) {
            continue;
        }
        _arr[i] = new node(v._arr[i]->_key, v._arr[i]->_is_del);
    }
}

hash_table::~hash_table() {
    for (auto & item : _arr) {
        delete item;
    }
    _arr.clear();
}

void hash_table::add(token value) {
    if (REHASH <= (_size * 1.0 / _arr.size())) {
        rehash();
    }
    size_t idx = hash_function(value.text(), _arr.size());
    while (_arr[idx] != nullptr && ! _arr[idx]->_is_del) {
        if (_arr[idx]->_key == value) {
            return;
        }
        idx = (idx + 1) % _arr.size();
    }
    _size++;
    if (_arr[idx] == nullptr) {
        _arr[idx] = new node(value, false);
    } else {
        _arr[idx]->_key = value;
        _arr[idx]->_is_del = false;
    }
}

void hash_table::remove(token value) {
    size_t idx = hash_function(value.text(), _arr.size());
    while (_arr[idx] != nullptr) {
        if (_arr[idx]->_key == value) {
            _arr[idx]->_key = token();
            _arr[idx]->_is_del = true;
            _size--;
            return;
        }
        idx++;
    }
}

type_lexeme hash_table::find_lexeme(std::string& text) {
    size_t idx = hash_function(text, _arr.size());
    while (_arr[idx] != nullptr && _arr[idx]->_key.text() != text) {
        idx = (idx + 1) % _arr.size();
    }
    if (_arr[idx] != nullptr && _arr[idx]->_key.text() == text && ! _arr[idx]->_is_del) {
        return _arr[idx]->_key.type();
    }
    return UNKNOWN;
}

void hash_table::print() {
    for (int i = 0; i < _arr.size(); i++) {
        if (_arr[i] == nullptr || _arr[i]->_is_del) {
            continue;
        }
        std::cout << "Key: " << i << "\tvalue: " << _arr[i]->_key << '\n';
    }
}

std::vector<std::pair<size_t, token>> hash_table::to_array() const {
    std::vector<std::pair<size_t, token>> res;
    for (size_t i = 0; i < _arr.size(); i++) {
        if (_arr[i] == nullptr || _arr[i]->_is_del) {
            continue;
        }
        res.emplace_back(i, _arr[i]->_key);
    }
    return res;
}