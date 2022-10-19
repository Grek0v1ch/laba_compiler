#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <iostream>
#include <string>
#include <vector>
#include "DFSM.h"
#include "../data_structures/hash_table.h"

// Структура лексемы.
struct lexeme {
	// Тип лексемы.
	type_lex _id;
	// Сама лексема.
	std::string _text;

	lexeme(const type_lex& id, const std::string& text) : _id(id), _text(text) {}
	lexeme(const type_lex&, const char);

	bool operator==(const lexeme& first) const { return first._id == _id && first._text == _text; }
	friend std::ostream& operator<<(std::ostream&, const lexeme&);
};

// Структура - обертка над хеш-функцией.
struct hash_function_lex {
	static const int _key = 53;

	// Хеш-функция для строк.
	int hash_function(const std::string&, const int, const int) const;
	// Переопределение оператора () для корректной работы хеш-таблицы.
	int operator()(const lexeme&, const int) const;
};

// Класс лексического анализатора.
class lexical_analyzer {
public:
	// Класс имеет всего лишь один метод, который принимает текст и возвращает хеш-таблицу
	// лексем.
	static hash_table<lexeme, hash_function_lex> lex_analize(std::string);
};

#endif // LEXICAL_ANALYZER_H
