#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <string>
#include <vector>
#include "linked_list.h"

// Класс хеш-таблицы.
// В качестве второго передаваемого класса следует указатель класс-обертку над хеш-функцией.
// В классе обертке необходима переадресация оператора ().
template <class T, class T_hash>
class hash_table {
private:
	static const size_t DEFAULT_SIZE = 7;
	std::vector<std::shared_ptr<linked_list<T>>> _arr;
	size_t _size;
public:
	hash_table() : _arr(std::vector<std::shared_ptr<linked_list<T>>>(DEFAULT_SIZE, 0)), _size(0) {}

	// Метод добавления элемента в хеш-таблицу.
	void add(const T& value) {
		T_hash hash = T_hash();
		// Вычисляем хеш элемента.
		int idx = hash(value, _arr.size());
		if (_arr[idx] == 0) {
			_size++;
			_arr[idx] = std::make_shared<linked_list<T>>(linked_list<T>(value));
			return;
		}
		if (_arr[idx]->find(value)) {
			return;
		}
		_size++;
		_arr[idx]->push_front(value);
	}

	// Метод удаления элемента из хеш-таблицы по значению.
	void remove(const T& value) {
		T_hash hash = T_hash();
		// Вычисляем хеш элемента.
		int idx = hash(value, _arr.size());
		// Далее пытаемся найти элемент по его хешу, если нашли - удаляем.
		if (_arr[idx] == 0) {
			return;
		}
		if (_arr[idx]->find(value)) {
			_size--;
			_arr[idx]->remove(value);
			// Если в ячейке по данному хешу был только один элемент, то зануляем ячейку.
			// Это необходимо для корректной работы функции печати.
			if (_arr[idx]->empty()) {
				_arr[idx] = 0;
			}
		}
	}

	// Метод печати хеш-таблицы в консоль.
	void print() {
		for (size_t i = 0; i < _arr.size(); i++) {
			if (_arr[i] == 0) {
				continue;
			}
			pnode<T> curr = _arr[i]->front();
			while (curr) {
				std::cout << curr->value() << "   key: " << i << '\n';
				curr = curr->next();
			}
		}
	}

	std::vector<std::pair<size_t, T>> to_array() {
		std::vector<std::pair<size_t, T>> res;
		for (size_t i = 0; i < _arr.size(); i++) {
			if (_arr[i] == 0) {
				continue;
			}
			pnode<T> curr = _arr[i]->front();
			while (curr) {
				res.push_back(std::make_pair(i, curr->value()));
				curr = curr->next();
			}
		}
		return res;
	}
};

#endif // HASH_TABLE_H
