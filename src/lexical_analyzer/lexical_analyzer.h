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
    std::string _text;
    size_t _idx;

    bool is_separators(char s);
    bool is_separators(std::string& s);
    bool is_keyword(std::string& s);
    type_lexeme get_separator_type(std::string& s);
    type_lexeme get_keyword_type(std::string& s);
    std::string get_next_word();
public:
    lexical_analyzer(std::string& text) : _text(std::move(text)), _idx(0) {}
	// Класс имеет всего лишь один метод, который принимает текст и возвращает хеш-таблицу
	// лексем.
    hash_table lex_analyze(std::ofstream& fout);
};

#endif // LEXICAL_ANALYZER_H
