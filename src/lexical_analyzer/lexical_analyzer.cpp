#include "lexical_analyzer.h"
#include <cctype>
#include <algorithm>

void lexical_analyzer::update_position(const char s) {
    if (s == '\n') {
        _pos._data.push_back(0);
    } else {
        (*_pos._data.rbegin())++;
    }
    _pos._last_word += s;
}

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

// Метод возвращает тип лексемы разделителя (типы лексем объявлены в файле lexical_item.h)
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

// Метод возвращает тип ключевого слова (типы лексем объявлены в файле lexical_item.h)
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
    _pos._last_word.clear();
    while (true) {
        char s;
        _input.get(s);
        // Если это конец файла то останавливаем цикл
        if (_input.eof()) {
            break;
        }
        update_position(s);
        // Если это пробельный символ, то возвращаем накопленное слово.
        // Если накопленное слово пусто, то продолжаем накапливать
        if (isspace(s)) {
            if (word.empty()) {
                continue;
            }
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
            // Если был прочитан символ разделитель, то так как он является отдельным словом
            // его надо вернуть в поток и удалить из последнего прочитанного слова
            _input.unget();
            // Тут такой способ удаления, потому что erase удалял тупо последнюю ячейку
            // в массиве символов, а он почему-то состоит из 23 ячеек
            _pos._last_word = _pos._last_word.substr(0, _pos._last_word.size() - 1);
            // Уменьшаем позицию в строке
            (*_pos._data.rbegin())--;
            return word;
        }
        word += s;
    }
    return word;
}

lexical_analyzer::lexical_analyzer(const char* file_name) {
    std::ifstream fin(file_name);
    _input << fin.rdbuf();
    fin.close();
    _pos._data = std::vector<int>(1);
}

// Метод возвращает следующий по тексту токен
token lexical_analyzer::get_next_token() {
    std::string curr_word = get_next_word();
    if (curr_word.empty()) {
        return {};
    }
    if (is_separators(curr_word)) {
        _tokens.add({get_separator_type(curr_word), curr_word});
        return {get_separator_type(curr_word), curr_word};
    }
    if (is_keyword(curr_word)) {
        _tokens.add({get_keyword_type(curr_word), curr_word});
        return {get_keyword_type(curr_word), curr_word};
    }
    DFSM automat;
    _tokens.add({automat.process(curr_word), curr_word});
    return {automat.process(curr_word), curr_word};
}

void lexical_analyzer::return_last_word() {
    std::reverse(_pos._last_word.begin(), _pos._last_word.end());
    for (const char& s : _pos._last_word) {
        if (s == '\n' || (*_pos._data.rbegin()) == 0) {
            _pos._data.pop_back();
        } else {
            (*_pos._data.rbegin())--;
        }
        _input.putback(s);
    }
    if ((*_pos._data.rbegin()) == 0) {
        _pos._data.pop_back();
    }
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
