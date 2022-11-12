#include <string>
#include <vector>
#include <algorithm>
#include "hash_table.h"
#include "lexical_analyzer.h"

// Компаратор для сортировки вектора лексем по их типу.
bool cmp(const std::pair<size_t, token>& first, const std::pair<size_t, token>& second) {
	return first.second.type() < second.second.type();
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
    std::string all_text, buff;
    all_text = "";
	while (getline(fin, buff, '\n')) {
		all_text += buff + "\n";
	}
	return all_text;
}

// Функция выводит вектор лексем в файл.
void output_result(std::vector<std::pair<size_t, token>>& res_to_arr, std::ofstream& fout) {
	sort(res_to_arr.begin(), res_to_arr.end(), cmp);
	for (const auto& item : res_to_arr) {
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
    lexical_analyzer analyzer(argv[1]);
	hash_table res = analyzer.lex_analyze();
	std::vector<std::pair<size_t, token>> res_to_arr = res.to_array();
	output_result(res_to_arr, fout);
	
	fin.close();
	fout.close();
	return 0;
}
