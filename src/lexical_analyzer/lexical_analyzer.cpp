#include "lexical_analyzer.h"
#include <cctype>
#include <algorithm>

bool lexical_analyzer::is_separators(char s) {
    std::vector<char> spr_sym = { '+', '-', '(', ')', '{', '}', ',', ';', '=' };
    return std::find(spr_sym.begin(), spr_sym.end(), s) != spr_sym.end();
}

bool lexical_analyzer::is_separators(std::string& s) {
    std::vector<std::string> spr_sym = { "+", "-", "(", ")", "{", "}", ",", ";", "=" };
    return std::find(spr_sym.begin(), spr_sym.end(), s) != spr_sym.end();
}

bool lexical_analyzer::is_keyword(std::string& s) {
    return s == "int" || s == "return" || s == "char";
}

type_lexeme lexical_analyzer::get_separator_type(std::string& s) {
    if (s == "+") {
        return SUM;
    } else if (s == "-") {
        return MINUS;
    } else if (s == "(") {
        return LBRACKET;
    } else if (s == ")") {
        return RBRACKET;
    } else if (s == "{") {
        return LBRACKET_FIGURE;
    } else if (s == "}") {
        return RBRACKET_FIGURE;
    } else if (s == ",") {
        return COMMA;
    } else if (s == ";") {
        return SEMICOLON;
    } else if (s == "=") {
        return EQUALS;
    }
    return UNKNOWN;
}

type_lexeme lexical_analyzer::get_keyword_type(std::string& s) {
    if (s == "return") {
        return RETURN;
    } else if (s == "int") {
        return INT;
    } else if (s == "char") {
        return CHAR;
    }
    return UNKNOWN;
}

std::string lexical_analyzer::get_next_word() {
    std::string word;
    while (true) {
        char s;
        _input >> s;
        if (_input.eof()) {
            break;
        }
        if (isspace(s)) {
            return word;
        }
        if (is_separators(s)) {
            if (word.empty()) {
                word += s;
                return word;
            }
            return word;
        }
        word += s;
    }
    return word;
}

token lexical_analyzer::get_next_token() {
    std::string curr_word = get_next_word();
    if (curr_word.empty()) {
        return {};
    }
    if (is_separators(curr_word)) {
        return {get_separator_type(curr_word), curr_word};
    }
    if (is_keyword(curr_word)) {
        return {get_keyword_type(curr_word), curr_word};
    }
    DFSM automat;
    return {automat.process(curr_word), curr_word};
}

// Класс имеет всего лишь один метод, который принимает текст и возвращает хеш-таблицу
// лексем.
hash_table lexical_analyzer::lex_analyze() {
	hash_table res_table;
    DFSM automat = DFSM();
    while (true) {
        token curr = get_next_token();
        if (_input.eof()) {
            break;
        }
        if (curr.text().empty()) {
            continue;
        }
        res_table.add(curr);
    }
    return res_table;
}

