#include <string>
#include <vector>
#include <algorithm>
#include "hash_table.h"
#include "lexical_analyzer.h"

// Компаратор для сортировки вектора лексем по их типу.
bool cmp(const std::pair<size_t, token>& first, const std::pair<size_t, token>& second) {
	return first.second.type() < second.second.type();
}

bool is_open_input(const char* input) {
    std::ifstream fin(input);
    bool res = fin.is_open();
    fin.close();
    return res;
}

bool is_open_output(const char* output) {
    std::ofstream fout(output);
    bool res = fout.is_open();
    fout.close();
    return res;
}

// Функция проверяет, открылись ли файлы на чтение и запись.
bool open_file(int argc, char const *argv[]) {
	if (argc != 3) {
		return false;
	}
    return is_open_input(argv[1]) && is_open_output(argv[2]);
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
	if (! open_file(argc, argv)) {
		std::cout << "Enter one correct file for read and one correct file for write\n";
		return 0;
	}
    std::ofstream fout(argv[2]);
    lexical_analyzer analyzer(argv[1]);
	hash_table res = analyzer.get_all_tokens();
	std::vector<std::pair<size_t, token>> res_to_arr = res.to_array();
	output_result(res_to_arr, fout);
	
	fout.close();
	return 0;
}
