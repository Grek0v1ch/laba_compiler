#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "data_structures/hash_table.h"

enum type_lex {
	VAR,
	NUM,
	OPERATION,
	ERROR,
};

enum state {
	st_start,
	st_end_var,
	st_end_num,
	st_3
};

enum event {
	event_letter,
	event_digit,
	event_error
};

using table = std::vector<std::vector<state>>;

class Analize {
private:
	table _table;

	state function_step(const state from, const event curr_event) const {
		if (curr_event == event_error) {
			return from;
		}
		return _table[from][curr_event];
	}

	event get_event(const char symbol) const {
		if ('a' <= symbol && symbol <= 'z') {
			return event_letter;
		}
		if ('0' <= symbol && symbol <= '9') {
			return event_digit;
		}
		return event_error;
	}

public:
	Analize() : _table(table(4, std::vector<state>(2))) {
		_table[0][0] = st_end_var;
		_table[0][1] = st_end_num;

		_table[1][0] = st_end_var;
		_table[1][1] = st_end_var;

		_table[2][0] = st_3;
		_table[2][1] = st_end_num;

		_table[3][0] = st_3;
		_table[3][1] = st_3;
	}

	type_lex process(const std::string lex) const {
		state curr_state = st_start;
		for (char symbol : lex) {
			event curr_event = get_event(symbol);
			if (curr_event == event_error) {
				return ERROR;
			}
			curr_state = function_step(curr_state, curr_event);
		}
		if (curr_state == st_end_var) {
			return VAR;
		}
		if (curr_state == st_end_num) {
			return NUM;
		}
		return ERROR;
	}
};

struct lexeme {
	type_lex _id;
	std::string _text;

	lexeme(const type_lex& id, const std::string& text) : _id(id), _text(text) {}
	lexeme(const type_lex& id, const char text) : _id(id) {
		char* _txt = new char;
		*_txt = text;
		_text = std::string(_txt);
		delete _txt;
	}

	bool operator==(const lexeme& first) const {
		return first._id == _id && first._text == _text;
	}

	friend std::ostream& operator<<(std::ostream&, const lexeme&);
};

std::ostream& operator<<(std::ostream& out, const lexeme& lex) {
	std::string output ;
	switch (lex._id) {
		case VAR:
			output = "VAR";
			break;
		case NUM:
			output = "NUM";
			break;
		case OPERATION:
			output = "OPERATION";
			break;
		case ERROR:
			output = "ERROR";
			break;
	}
	out << output << "   " << lex._text;
	return out;
}

int hash_function_for_string(const std::string& s, const int table_size, const int key) {
    int hash_result = 0;
    for (unsigned int i = 0; i < s.length(); i++) {
        hash_result = (key * hash_result + s[i]) % table_size;
    }
    hash_result = (hash_result * 2 + 1) % table_size;
    return hash_result;
}

// Структура - обертка над хеш-функцией.
struct hash_function_lex {
	static const int _key = 53;
    int operator()(const lexeme& s, const int table_size) const {
        return hash_function_horner(s._text, table_size, _key);
    }
};

hash_table<lexeme, hash_function_lex> lex_analize(std::string str) {
	std::vector<char> space_symbol(3);
	space_symbol =  {' ', '\n', '\t'};
	std::vector<char> operation_symbol(2);
	operation_symbol = {'+', '-'};
	hash_table<lexeme, hash_function_lex> res_table;
	Analize dka = Analize();
	std::string temp = "";
	for (size_t i = 0; i < str.size(); i++) {
		bool is_space_sym = std::find(space_symbol.begin(),
									  space_symbol.end(), str[i]) != space_symbol.end();
		bool is_oper_sym = std::find(operation_symbol.begin(),
									 operation_symbol.end(), str[i]) != operation_symbol.end();
		if ((is_space_sym || is_oper_sym) && (temp != "")) {
			type_lex res = dka.process(temp);
			if (res == ERROR) {
				std::cout << "Error in " << temp << '\n';
				temp = "";
				continue;
			}
			res_table.add(lexeme(res, temp));
			temp = "";
		}
		if (! is_space_sym && ! is_oper_sym) {
			temp += str[i];
		}
		if (is_oper_sym) {
			res_table.add(lexeme(OPERATION, str[i]));
		}
	}
	return res_table;
}

int main() {
	std::ifstream fin("input.txt");
	std::string all_text = "";
	std::string buff;
	while (getline(fin, buff, '\n')) {
		all_text += buff + "\n";
	}
	hash_table<lexeme, hash_function_lex> res = lex_analize(all_text);
	res.print();
	fin.close();
	return 0;
}