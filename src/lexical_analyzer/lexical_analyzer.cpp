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
    for ( ; _idx < _text.size(); _idx++) {
        if (isspace(_text[_idx])) {
            _idx++;
            return word;
        }
        if (is_separators(_text[_idx])) {
            if (word.empty()) {
                word += _text[_idx];
                _idx++;
                return word;
            }
            return word;
        }
        word += _text[_idx];
    }
    return word;
}

// Класс имеет всего лишь один метод, который принимает текст и возвращает хеш-таблицу
// лексем.
hash_table lexical_analyzer::lex_analyze(std::ofstream& fout) {
	hash_table res_table;
    DFSM automat = DFSM();
    while (_idx < _text.size()) {
        std::string word = get_next_word();
        if (word.empty()) {
            continue;
        }
        if (is_separators(word)) {
            token temp(get_separator_type(word), word);
            res_table.add(temp);
        } else if (is_keyword(word)) {
            token temp(get_keyword_type(word), word);
            res_table.add(temp);
        } else {
            type_lexeme type = automat.process(word);
            if (type == UNKNOWN && _idx < _text.size()) {
                fout << "Error in " << word << '\n';
                continue;
            }
            token temp(type, word);
            res_table.add(temp);
        }
    }
    return res_table;
}

