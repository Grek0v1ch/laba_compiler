#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <fstream>
#include <string>
#include <vector>
#include "DFSM.h"
#include "token.h"
#include "hash_table.h"

// Класс лексического анализатора.
class lexical_analyzer {
    // Поле хранит файл из которого необходимо читать токены
    std::ifstream _input;
    hash_table _tokens;

    // Метод проверяет, что символ является символом разделителем (односимвольной лексемой)
    bool is_separators(char s);
    // Перегрузка метода для типа std::string
    bool is_separators(std::string& s);
    // Метод проверяет, является ли слово ключевым (это слова int, char, return)
    bool is_keyword(std::string& s);
    // Метод возвращает тип лексемы разделителя (типы лексем объявлены в файле token.h)
    type_lexeme get_separator_type(std::string& s);
    // Метод возвращает тип ключевого слова (типы лексем объявлены в файле token.h)
    type_lexeme get_keyword_type(std::string& s);
    // Метод получает следующее по тексту слово
    std::string get_next_word();
public:
    lexical_analyzer(const char* file_name) : _input(std::ifstream(file_name)) {}
    lexical_analyzer(std::string& file_name) : _input(std::ifstream(file_name)) {}
    ~lexical_analyzer() { _input.close(); }
    // Метод возвращает хеш-таблицу с токенами
    hash_table tokens() const { return _tokens; }
    // Метод возвращает следующий по тексту токен
    token get_next_token();
    // Метод возвращает хеш таблицу со всеми лексемами прочитанными из файла
    hash_table get_all_tokens();
};

#endif // LEXICAL_ANALYZER_H
