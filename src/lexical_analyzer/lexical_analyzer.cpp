#include "lexical_analyzer.h"
#include <cctype>
#include <algorithm>

// Метод проверяет, что символ является символом разделителем (односимвольной лексемой)
bool lexical_analyzer::is_separators(char s) {
    std::vector<char> spr_sym = { '+', '-', '(', ')', '{', '}', ',', ';', '=' };
    return std::find(spr_sym.begin(), spr_sym.end(), s) != spr_sym.end();
}

// Перегрузка метода для типа std::string
bool lexical_analyzer::is_separators(std::string& s) {
    std::vector<std::string> spr_sym = { "+", "-", "(", ")", "{", "}", ",", ";", "=" };
    return std::find(spr_sym.begin(), spr_sym.end(), s) != spr_sym.end();
}

// Метод проверяет, является ли слово ключевым (это слова int, char, return)
bool lexical_analyzer::is_keyword(std::string& s) {
    return s == "int" || s == "return" || s == "char";
}

// Метод возвращает тип лексемы разделителя (типы лексем объявлены в файле token.h)
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

// Метод возвращает тип ключевого слова (типы лексем объявлены в файле token.h)
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

// Метод получает следующее по тексту слово
std::string lexical_analyzer::get_next_word() {
    std::string word;
    while (true) {
        char s;
        _input.get(s);
        // Если это конец файла то останавливаем цикл
        if (_input.eof()) {
            break;
        }
        // Если это пробельный символ, то возвращаем накопленное слово
        if (isspace(s)) {
            return word;
        }
        // Если это односимвольная лексема, то тогда если накопленное слово пустое, то возвращаем
        // эту односимвольную лексему. Если же уже успело накопиться какое-то слово, то возвращаем
        // прочитанный символ в поток(так как этот символ отдельная лексема) и возвращаем
        // накопленное слово
        if (is_separators(s)) {
            if (word.empty()) {
                word += s;
                return word;
            }
            _input.unget();
            return word;
        }
        word += s;
    }
    return word;
}

// Метод возвращает следующий по тексту токен
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

// Метод возвращает хеш таблицу со всеми лексемами прочитанными из файла
hash_table lexical_analyzer::get_all_tokens() {
    DFSM automat = DFSM();
    while (true) {
        token curr = get_next_token();
        if (_input.eof()) {
            break;
        }
        if (curr.text().empty()) {
            continue;
        }
        _tokens.add(curr);
    }
    return _tokens;
}

