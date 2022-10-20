#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "data_structures/hash_table.h"
#include "lexical_analyzer/DFSM.h"
#include "lexical_analyzer/lexical_analyzer.h"

// Компоратор для сортировки вектора лексем по их типу.
bool cmp(const std::pair<size_t, lexeme>& first, const std::pair<size_t, lexeme>& second) {
	return first.second._id < second.second._id;
}

// Функция проверяет, открылись ли файлы на чтение и запись.
bool open_file(int argc, char const *argv[], std::ifstream& fin, std::ofstream& fout) {
	if (argc != 3) {
		return false;
	}
	fin = std::ifstream(argv[1]);
	fout = std::ofstream(argv[2]);
	return fin.is_open() && fout.is_open();
}

// Функция получает весь текст из файла.
std::string get_all_text(std::ifstream& fin) {
	std::string all_text = "";
	std::string buff;
	while (getline(fin, buff, '\n')) {
		all_text += buff + "\n";
	}
	return all_text;
}

// Функция выводит вектор лексем в файл.
void output_result(std::vector<std::pair<size_t, lexeme>>& res_to_arr, std::ofstream& fout) {
	sort(res_to_arr.begin(), res_to_arr.end(), cmp);
	for (auto item : res_to_arr) {
		fout << "Key: " << item.first << ' ' << item.second << '\n';
	}
}

int main(int argc, char const *argv[]) {
	std::ifstream fin;
	std::ofstream fout;
	if (! open_file(argc, argv, fin, fout)) {
		std::cout << "Enter one correct file for read and one correct file for write\n";
		return 0;
	}

	std::string all_text = get_all_text(fin);
	hash_table<lexeme, hash_function_lex> res = lexical_analyzer::lex_analize(all_text, fout);
	std::vector<std::pair<size_t, lexeme>> res_to_arr = res.to_array();
	output_result(res_to_arr, fout);
	
	fin.close();
	fout.close();
	return 0;
}
