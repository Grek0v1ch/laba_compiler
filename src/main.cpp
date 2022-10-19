#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include "data_structures/hash_table.h"

enum type_lex {
	KEYWORD,
	ID,
	NUM,
	SEPARATORS,
	STRING,
	ERROR
};

class DFSM {
private:
	enum state {
		st_0_start,
		st_1_end_id,
		st_2_end_num,
		st_3,
		st_4,
		st_5_end_string
	};

	enum event {
		event_letter,
		event_digit,
		event_quotes,
		event_error
	};

	using table = std::vector<std::vector<state>>;

	#define COUNT_STATE 6
	#define COUNT_EVENT 3

	table _table;

	state function_step(const state from, const event curr_event) const {
		if (curr_event == event_error) {
			return from;
		}
		return _table[from][curr_event];
	}

	event get_event(const char symbol) const {
		if (isalpha(symbol)) {
			return event_letter;
		}
		if (isdigit(symbol)) {
			return event_digit;
		}
		if (symbol == '"') {
			return event_quotes;
		}
		return event_error;
	}

public:
	DFSM() : _table(table(COUNT_STATE, std::vector<state>(COUNT_EVENT))) {
		_table[st_0_start][event_letter] = st_1_end_id;
		_table[st_0_start][event_digit] = st_2_end_num;
		_table[st_0_start][event_quotes] = st_3;

		_table[st_1_end_id][event_letter] = st_1_end_id;
		_table[st_1_end_id][event_digit] = st_4;
		_table[st_1_end_id][event_quotes] = st_4;

		_table[st_2_end_num][event_letter] = st_4;
		_table[st_2_end_num][event_digit] = st_2_end_num;
		_table[st_2_end_num][event_quotes] = st_4;

		_table[st_3][event_letter] = st_3;
		_table[st_3][event_digit] = st_3;
		_table[st_3][event_quotes] = st_5_end_string;

		_table[st_4][event_letter] = st_4;
		_table[st_4][event_digit] = st_4;
		_table[st_4][event_quotes] = st_4;

		_table[st_5_end_string][event_letter] = st_4;
		_table[st_5_end_string][event_digit] = st_4;
		_table[st_5_end_string][event_quotes] = st_4;
	}

	type_lex process(const std::string lex) const {
		state curr_state = st_0_start;
		for (char symbol : lex) {
			event curr_event = get_event(symbol);
			if (curr_event == event_error) {
				return ERROR;
			}
			curr_state = function_step(curr_state, curr_event);
		}
		switch (curr_state) {
			case st_1_end_id:
				return ID;
			case st_2_end_num:
				return NUM;
			case st_5_end_string:
				return STRING;
			default:
				return ERROR;
		}
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
		case KEYWORD:
			output = "Keyword";
			break;
		case ID:
			output = "Id";
			break;
		case NUM:
			output = "Num";
			break;
		case SEPARATORS:
			output = "Separators";
			break;
		case STRING:
			output = "SimpleStringExpresion";
			break;
		case ERROR:
			output = "Error";
			break;
	}
	out << output << ' ' << lex._text;
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
	std::vector<char> space_symbol = { ' ', '\n', '\t' };
	std::vector<char> separation_symbol = { '+', '-', '(', ')', '{', '}', ',', ';', '=' };
	std::vector<std::string> keywords = { "return", "int", "char" };
	hash_table<lexeme, hash_function_lex> res_table;
	DFSM automat = DFSM();
	std::string temp = "";
	for (size_t i = 0; i < str.size(); i++) {
		bool is_space_sym = std::find(space_symbol.begin(), space_symbol.end(),
									  str[i]) != space_symbol.end();
		bool is_separation_symbol = std::find(separation_symbol.begin(), separation_symbol.end(),
											  str[i]) != separation_symbol.end();
		if ((is_space_sym || is_separation_symbol) && (temp != "")) {
			if (std::find(keywords.begin(), keywords.end(), temp) != keywords.end()) {
				res_table.add(lexeme(KEYWORD, temp));
				temp = "";
				continue;
			}
			type_lex res = automat.process(temp);
			if (res == ERROR) {
				std::cout << "Error in " << temp << '\n';
				temp = "";
				continue;
			}
			res_table.add(lexeme(res, temp));
			temp = "";
		}
		if (! is_space_sym && ! is_separation_symbol) {
			temp += str[i];
		}
		if (is_separation_symbol) {
			res_table.add(lexeme(SEPARATORS, str[i]));
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