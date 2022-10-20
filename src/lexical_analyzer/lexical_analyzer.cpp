#include "lexical_analyzer.h"

lexeme::lexeme(const type_lex& id, const char text) : _id(id) {
	char* _txt = new char;
	*_txt = text;
	_text = std::string(_txt);
	delete _txt;
}

std::ostream& operator<<(std::ostream& out, const lexeme& lex) {
	std::string output ;
	switch (lex._id) {
		case KEYWORD:
			output = "Keyword";
			break;
		case ID:
			output = "Id";
			break;
		case NUM:
			output = "Num";
			break;
		case SEPARATORS:
			output = "Separators";
			break;
		case STRING:
			output = "SimpleStringExpresion";
			break;
		case ERROR:
			output = "Error";
			break;
	}
	out << output << ' ' << lex._text;
	return out;
}

// Хеш-функция для строк.
int hash_function_lex::hash_function(const std::string& s, const int table_size, const int key) const {
    int hash_result = 0;
    for (unsigned int i = 0; i < s.length(); i++) {
        hash_result = (key * hash_result + s[i]) % table_size;
    }
    hash_result = (hash_result * 2 + 1) % table_size;
    return hash_result;
}

int hash_function_lex::operator()(const lexeme& s, const int table_size) const {
    return hash_function(s._text, table_size, _key);
}

// Класс имеет всего лишь один метод, который принимает текст и возвращает хеш-таблицу
// лексем.
hash_table<lexeme, hash_function_lex> lexical_analyzer::lex_analize(std::string str, std::ofstream& fout) {
	// Массив пробельных символов.
	std::vector<char> spc_sym = { ' ', '\n', '\t' };
	// Массив односимвольных лексем, которые используются в качестве разделителей.
	std::vector<char> spr_sym = { '+', '-', '(', ')', '{', '}', ',', ';', '=' };
	// Массив ключевых слов языка.
	std::vector<std::string> keywords = { "return", "int", "char" };
	hash_table<lexeme, hash_function_lex> res_table;
	DFSM automat = DFSM();
	std::string temp = "";
	for (size_t i = 0; i < str.size(); i++) {
		// Сначала проверяем не является-ли текущий символ пробельным или разделителем.
		bool is_spc_sym = std::find(spc_sym.begin(), spc_sym.end(), str[i]) != spc_sym.end();
		bool is_spr_sym = std::find(spr_sym.begin(), spr_sym.end(), str[i]) != spr_sym.end();
		
		if ((is_spc_sym || is_spr_sym) && (temp != "")) {
			// В переменной temp накоплено слово. Проверяем, не ключевое-ли оно.
			if (std::find(keywords.begin(), keywords.end(), temp) != keywords.end()) {
				res_table.add(lexeme(KEYWORD, temp));
				temp = "";
				continue;
			}
			// Если не ключевое, то запускаем автомат.
			type_lex res = automat.process(temp);
			if (res == ERROR) {
				fout << "Error in " << temp << '\n';
				temp = "";
				continue;
			}
			res_table.add(lexeme(res, temp));
			temp = "";
		}
		// Если не пробельный и не разделитель, то накапливаем слово дальше.
		if (! is_spc_sym && ! is_spr_sym) {
			temp += str[i];
		}
		// Разделители тоже явялются лексемами, добавляем их.
		if (is_spr_sym) {
			res_table.add(lexeme(SEPARATORS, str[i]));
		}
	}
	return res_table;
}

