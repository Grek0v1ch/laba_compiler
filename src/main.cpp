#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "data_structures/hash_table.h"
#include "lexical_analyzer/DFSM.h"
#include "lexical_analyzer/lexical_analyzer.h"

bool cmp(const std::pair<size_t, lexeme>& first, const std::pair<size_t, lexeme>& second) {
	return first.second._id < second.second._id;
}

int main() {
	std::ifstream fin("input.txt");
	std::string all_text = "";
	std::string buff;
	while (getline(fin, buff, '\n')) {
		all_text += buff + "\n";
	}
	hash_table<lexeme, hash_function_lex> res = lexical_analyzer::lex_analize(all_text);
	std::vector<std::pair<size_t, lexeme>> res_to_arr = res.to_array();
	sort(res_to_arr.begin(), res_to_arr.end(), cmp);
	for (auto item : res_to_arr) {
		std::cout << "Key: " << item.first << ' ' << item.second << '\n';
	}
	fin.close();
	return 0;
}