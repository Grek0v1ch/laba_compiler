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
