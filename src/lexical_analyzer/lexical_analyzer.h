#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <iostream>
#include <string>
#include <vector>
#include "DFSM.h"
#include "../data_structures/hash_table.h"

struct lexeme {
	type_lex _id;
	std::string _text;

	lexeme(const type_lex& id, const std::string& text) : _id(id), _text(text) {}
	lexeme(const type_lex&, const char);

	bool operator==(const lexeme& first) const { return first._id == _id && first._text == _text; }
	friend std::ostream& operator<<(std::ostream&, const lexeme&);
};

// Структура - обертка над хеш-функцией.
struct hash_function_lex {
	static const int _key = 53;

	int hash_function(const std::string&, const int, const int) const;
	int operator()(const lexeme&, const int) const;
};

class lexical_analyzer {
public:
	static hash_table<lexeme, hash_function_lex> lex_analize(std::string str) {
		std::vector<char> spc_sym = { ' ', '\n', '\t' };
		std::vector<char> spr_sym = { '+', '-', '(', ')', '{', '}', ',', ';', '=' };
		std::vector<std::string> keywords = { "return", "int", "char" };
		hash_table<lexeme, hash_function_lex> res_table;
		DFSM automat = DFSM();
		std::string temp = "";
		for (size_t i = 0; i < str.size(); i++) {
			bool is_spc_sym = std::find(spc_sym.begin(), spc_sym.end(), str[i]) != spc_sym.end();
			bool is_spr_sym = std::find(spr_sym.begin(), spr_sym.end(), str[i]) != spr_sym.end();
			
			if ((is_spc_sym || is_spr_sym) && (temp != "")) {
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
			if (! is_spc_sym && ! is_spr_sym) {
				temp += str[i];
			}
			if (is_spr_sym) {
				res_table.add(lexeme(SEPARATORS, str[i]));
			}
		}
		return res_table;
	}
};

#endif // LEXICAL_ANALYZER_H
