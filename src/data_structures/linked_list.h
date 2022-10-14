#ifndef LINKED_LIST_H
#define LINKED_LIST_H

// Класс узла связанного списка.
template <class T>
class node {
private:
	T _value;
	std::shared_ptr<node<T>> _next;
public:
	node(const T& value, const std::shared_ptr<node<T>>& next = 0) : _value(value), _next(next) {}
	// Класс связанного списка объявлен дружеским, чтобы иметь доступ к его приватным членам
	// из класса связанного списка.
	template <class T1> friend class linked_list;
};

// Объявляем новый тип данных, который является указателем на узел.
template <class T>
using pnode = std::shared_ptr<node<T>>;

// Класс связанного списка.
template <class T>
class linked_list {
private:
	// Указатель на первый элемент списка.
	pnode<T> _first;
public:
	linked_list() : _first(0) {}
	linked_list(const T& value) : _first(pnode<T>(node<T>(value))) {}

	// Метод проверяет, является ли список пустым.
	bool empty() const { return _first == 0; }

	// Метод добавляет новый элемент в начало списка.
	void push_front(const T& value) {
		pnode<T> new_el = std::make_shared<node<T>>(node<T>(value));
		// Если список пуст, то новый элемент равен первому.
		if (empty()) {
			_first = new_el;
			return;
		}
		// Смещаем первый элемент на следующее место после нового.
		new_el->_next = _first;
		_first = new_el;
	}

	// Метод поиска значения в списке.
	bool find(const T& value) const {
		pnode<T> curr = _first;
		while (curr) {
			if (curr->_value == value) {
				return true;
			}
			curr = curr->_next;
		}
		return false;
	}

	// Метод печати списка в консоль. После печати переход на новую строку не осуществляется.
	// Выводит значения в виде: value1 value2 value3 ...
	void print() const {
		pnode<T> curr = _first;
		while (curr) {
			std::cout << curr->_value << ' ';
			curr = curr->_next;
		}
	}
};

#endif // LINKED_LIST_H
