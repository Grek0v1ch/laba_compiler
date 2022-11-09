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

    std::string get_next_word();
public:
    lexical_analyzer(std::string& text) : _text(std::move(text)), _idx(0) {}
	// Класс имеет всего лишь один метод, который принимает текст и возвращает хеш-таблицу
	// лексем.
    hash_table lex_analyze(std::ofstream& fout);
};

#endif // LEXICAL_ANALYZER_H
