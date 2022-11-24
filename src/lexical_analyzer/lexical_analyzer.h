#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "DFSM.h"
#include "token.h"
#include "hash_table.h"

/**
 * Структура позиции в файле. Содержит в себе вектор с количеством символов в каждой строке и
 * последнее прочитанное слово, чтобы возможно было осуществлять корректный откат
 * */
struct position {
    // В элементе вектора с индексом i лежит количество символов в строке с номером i + 1.
    std::vector<int> _data;
    // Последнее прочитанное слово.
    std::string _last_word;
};

/**
 * Класс лексического анализатора.
 * */
class lexical_analyzer {
private:
    // Поле хранит поток из которого необходимо читать токены
    std::stringstream _input;
    // Таблица со всеми прочитанными токенами
    hash_table _tokens;
    // Текущая позиция
    position _pos;

    /**
     * Метод обновляет позицию в файле в зависимости от текущего символа
     * \param s - символ по которому обновляется позиция в файле
     * */
    void update_position(char s);
    /**
     * Метод проверяет, что символ является символом разделителем (односимвольной лексемой)
     * \param s - символ для проверки
     * */
    bool is_separators(char s);
    /**
     * Метод проверяет, что символ является символом разделителем (односимвольной лексемой)
     * \param s - символ для проверки
     * */
    bool is_separators(std::string& s);
    /**
     * Метод проверяет, является ли слово ключевым (это слова int, char, return)
     * \param s - слово для проверки
     * */
    bool is_keyword(std::string& s);
    /**
     * Метод возвращает тип токена разделителя
     * \param s - слово для определения типа
     * */
    token::type_lexeme get_separator_type(std::string& s);
    /**
     * Метод возвращает тип ключевого слова
     * \param s - слово для определения типа
     * */
    token::type_lexeme get_keyword_type(std::string& s);
    /**
     * Метод получает следующее по тексту слово
     * */
    std::string get_next_word();
public:
    /**
     * Конструктор по имени файла. Файл переводится в поток stringstream
     * \param file_name - файл для чтения
     * */
    lexical_analyzer(const char* file_name);
    /**
     * Деструктор. В нем закрывается поток stringstream
     * */
    ~lexical_analyzer() { _input.clear(); }
    /**
     * Метод возвращает хеш-таблицу с токенами
     * */
    hash_table tokens() const { return _tokens; }
    /**
     * Метод возвращает текущую позицию
     * */
    position position() const { return _pos; }
    /**
     * Метод возвращает следующий по тексту токен
     * */
    token get_next_token();
    /**
     * Метод возвращает в поток последнее прочитанное слово
     * */
    void return_last_word();
    /**
     * Метод запускает лексический анализ по всему файлу. После его работы файл будет полностью
     * прочитан
     * */
    hash_table get_all_tokens();
};

#endif // LEXICAL_ANALYZER_H
