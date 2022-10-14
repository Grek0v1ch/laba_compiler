#ifndef LINKED_LIST_H
#define LINKED_LIST_H

template <class T>
class linked_list;

template <class T>
class node {
private:
	T _value;
	std::shared_ptr<T> _next;
public:
	node(const T& value, const std::shared_ptr<T>& next = 0) : _value(value), _next(next) {}
	template <class T1> friend class linked_list;
};

#endif // LINKED_LIST_H
