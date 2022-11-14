#include <vector>
#include <algorithm>
#include "hash_table.h"
#include "lexical_analyzer.h"
#include "parser.h"

// Функция проверяет, можно ли открыть файл на запись
bool is_open_output(const char* output);
// Функция проверяет можно, ли открыть файл на чтение
bool is_open_input(const char* input);
// Функция проверяет, можно ли из аргументов командной строки открыть два файла:
// первый на чтение, а второй на запись
bool open_file(int argc, char const *argv[]);
// Функция сравнения для сортировки вектора с токенами
bool cmp(const std::pair<size_t, token>& first, const std::pair<size_t, token>& second);
// Функция выводит в файл список полученных токенов
void output_result(std::vector<std::pair<size_t, token>>& res_to_arr, std::ofstream& fout);

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

    parser par(argv[1]);
    par.run();

	fout.close();
	return 0;
}

// Функция проверяет, можно ли открыть файл на запись
bool is_open_output(const char* output) {
    std::ofstream fout(output);
    bool res = fout.is_open();
    fout.close();
    return res;
}

// Функция проверяет можно, ли открыть файл на чтение
bool is_open_input(const char* input) {
    std::ifstream fin(input);
    bool res = fin.is_open();
    fin.close();
    return res;
}

// Функция проверяет, можно ли из аргументов командной строки открыть два файла:
// первый на чтение, а второй на запись
bool open_file(int argc, char const *argv[]) {
	if (argc != 3) {
		return false;
	}
    return is_open_input(argv[1]) && is_open_output(argv[2]);
}

// Функция сравнения для сортировки вектора с токенами
bool cmp(const std::pair<size_t, token>& first, const std::pair<size_t, token>& second) {
    return first.second.type() < second.second.type();
}

// Функция выводит в файл список полученных токенов
void output_result(std::vector<std::pair<size_t, token>>& res_to_arr, std::ofstream& fout) {
	sort(res_to_arr.begin(), res_to_arr.end(), cmp);
	for (const auto& item : res_to_arr) {
		fout << "Key: " << item.first << ' ' << item.second << '\n';
	}
}
