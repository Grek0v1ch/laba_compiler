#include <fstream>
#include <string>
#include "data_structures/hash_table.h"
#include "lexical_analyzer/DFSM.h"
#include "lexical_analyzer/lexical_analyzer.h"

int main() {
	std::ifstream fin("input.txt");
	std::string all_text = "";
	std::string buff;
	while (getline(fin, buff, '\n')) {
		all_text += buff + "\n";
	}
	hash_table<lexeme, hash_function_lex> res = lexical_analyzer::lex_analize(all_text);
	res.print();
	fin.close();
	return 0;
}