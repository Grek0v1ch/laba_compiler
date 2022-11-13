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
    std::ifstream _input;

    bool is_separators(char s);
    bool is_separators(std::string& s);
    bool is_keyword(std::string& s);
    type_lexeme get_separator_type(std::string& s);
    type_lexeme get_keyword_type(std::string& s);
    std::string get_next_word();
public:
    lexical_analyzer(const char* file_name) : _input(std::ifstream(file_name)) {}
    lexical_analyzer(std::string& file_name) : _input(std::ifstream(file_name)) {}
    ~lexical_analyzer() { _input.close(); }
	// Класс имеет всего лишь один метод, который принимает текст и возвращает хеш-таблицу
	// лексем.
    hash_table get_all_tokens();
    token get_next_token();
};

#endif // LEXICAL_ANALYZER_H
