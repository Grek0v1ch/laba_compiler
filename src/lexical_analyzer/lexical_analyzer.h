#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "DFSM.h"
#include "token.h"
#include "hash_table.h"

struct position {
    std::vector<int> _data;
    std::string _last_word;
};

// Класс лексического анализатора.
class lexical_analyzer {
    // Поле хранит файл из которого необходимо читать токены
    std::stringstream _input;
    hash_table _tokens;
    position _pos;

    // Метод обновляет позицию в файле в зависимости от текущего символа
    void update_position(char s);
    // Метод проверяет, что символ является символом разделителем (односимвольной лексемой)
    bool is_separators(char s);
    // Перегрузка метода для типа std::string
    bool is_separators(std::string& s);
    // Метод проверяет, является ли слово ключевым (это слова int, char, return)
    bool is_keyword(std::string& s);
    // Метод возвращает тип лексемы разделителя (типы лексем объявлены в файле lexical_item.h)
    type_lexeme get_separator_type(std::string& s);
    // Метод возвращает тип ключевого слова (типы лексем объявлены в файле lexical_item.h)
    type_lexeme get_keyword_type(std::string& s);
    // Метод получает следующее по тексту слово
    std::string get_next_word();
public:
    lexical_analyzer(const char* file_name);
    ~lexical_analyzer() { _input.clear(); }
    // Метод возвращает хеш-таблицу с токенами
    hash_table tokens() const { return _tokens; }
    position position() const { return _pos; }
    // Метод возвращает следующий по тексту токен
    token get_next_token();
    // Метод записывает токен обратно в поток ввода, разделяя токен и оставшийся поток ввода
    // пробелом
    void return_last_word();
    // Метод возвращает хеш таблицу со всеми лексемами прочитанными из файла
    hash_table get_all_tokens();
};

#endif // LEXICAL_ANALYZER_H
