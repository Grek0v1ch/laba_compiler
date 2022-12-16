#include "hash_table.h"

// Метод ре-хеширования (увеличения размера выделенной памяти)
void hash_table::rehash() {
    // Обнуляем размер, так как мы по новой будем добавлять узлы в таблицу
    _size = 0;
    // Создаем вектор копию, так как основной вектор необходимо увеличить в 2 раза и занулить
    std::vector<pnode> arr(_arr);
    std::fill(_arr.begin(),  _arr.end(), nullptr);
    _arr.resize(_arr.size() * 2, nullptr);
    for (auto & item : arr) {
        // Если это существующий и не удаленный элемент, то добавляем его
        if (item != nullptr && ! item->_is_del) {
            add(item->_key);
        }
    }
    // Чистим память
    for (auto & item : arr) {
        delete item;
    }
}

// Хеш-функция для строк.
// Так как хеш-таблица хранит токены, а у токенов есть текстовое представление, то по этому
// текстовому представлению и получаем хеш
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
    _arr = std::vector<pnode>(v._arr.size(), nullptr);
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
    // Проверяем, не нужно ли делать ре-хеширование
    if (REHASH <= (_size * 1.0 / _arr.size())) {
        rehash();
    }
    size_t idx = hash_function(value.text(), _arr.size());
    // Ищем место, куда можно вставить элемент.
    // Это должен быть либо пустой, либо удаленный узел
    while (_arr[idx] != nullptr && ! _arr[idx]->_is_del) {
        // Так как хеш-таблица не допускает повторов, то выходим, как только нашли такой же элемент
        if (_arr[idx]->_key == value) {
            return;
        }
        idx = (idx + 1) % _arr.size();
    }
    _size++;
    // Вставка происходить по-разному: если это пустой узел, то просто создаем новый, а если это
    // удаленный узел, то заменяем значение в нем и опускаем флаг
    if (_arr[idx] == nullptr) {
        _arr[idx] = new node(value, false);
    } else {
        _arr[idx]->_key = value;
        _arr[idx]->_is_del = false;
    }
}

void hash_table::remove(token value) {
    size_t idx = hash_function(value.text(), _arr.size());
    // В случае удаления просто идем пока не встретим пустой узел.
    // Если элемент уже был удален, то ничего не поменяется
    while (_arr[idx] != nullptr) {
        if (_arr[idx]->_key == value) {
            _arr[idx]->_is_del = true;
            _size--;
            return;
        }
        idx++;
    }
}

 void hash_table::set_type(std::string& text, std::string& var_type) {
    size_t idx = hash_function(text, _arr.size());
    // Идем по таблице пока не упремся либо в пустой узел, либо в узел текстовое представление
    // которого совпадает с искомым текстовым представлением
    while (_arr[idx] != nullptr && _arr[idx]->_key.text() != text) {
        idx = (idx + 1) % _arr.size();
    }
    // Меняем токен только в том случае, если мы нашли не пустой и не удаленный узел,
    // текстовое представление токена которого равно искомому текстовому представлению
    if (_arr[idx] != nullptr && _arr[idx]->_key.text() == text && ! _arr[idx]->_is_del) {
        _arr[idx]->_key.set_var_type(var_type);
    }
}

// Метод печати хеш-таблицы в консоль
void hash_table::print() {
    for (int i = 0; i < _arr.size(); i++) {
        if (_arr[i] == nullptr || _arr[i]->_is_del) {
            continue;
        }
        std::cout << "Key: " << i << "\tvalue: " << _arr[i]->_key << '\n';
    }
}

// Метод возвращает токен с данным текстовым представлением
std::vector<std::pair<size_t, token>> hash_table::to_array() const {
    std::vector<std::pair<size_t, token>> res;
    for (size_t i = 0; i < _arr.size(); i++) {
        // Добавляем только не пустые и не удаленные узлы
        if (_arr[i] == nullptr || _arr[i]->_is_del) {
            continue;
        }
        res.emplace_back(i, _arr[i]->_key);
    }
    return res;
}