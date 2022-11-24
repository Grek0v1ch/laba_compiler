#ifndef DFSM_H
#define DFSM_H

#include <vector>
#include <string>
#include "token.h"

/**
 * Класс детерминированного конечного автомата для распознавания токенов языка.
 */
class DFSM {
private:
	// Состояния автомата.
	enum state {
		st_0_start,
		st_1_end_id,
		st_2_end_num,
		st_3,
		st_4,
		st_5_end_string
	};

	// Значения текущего символа строки.
	enum event {
		event_letter,
		event_digit,
		event_quotes,
		event_error
	};

	using table = std::vector<std::vector<state>>;

    // Размеры таблицы состояний автомата
	#define COUNT_STATE 6
	#define COUNT_EVENT 3

	table _table;

	/**
	 * Метод совершает "шаг" автомата - из состояния по символу переходит в новое состояние.
	 * \param from - состояние из которого делаем шаг
	 * \param curr_event - по какому событию делаем переход
	 * */
	state function_step(state from, event curr_event) const;
	/**
	 * Метод по символу определяет событие в автомате
	 * \param s - символ для определения состояния
	 * */
	event get_event(char s) const;

public:
    /**
     * Конструктор по умолчанию
     * */
	DFSM();
	/**
	 * Метод определяет тип токена по его текстовому представлению
	 * \param lex - текстовое представление токена
	 * */
	token::type_lexeme process(const std::string& lex) const;
};

#endif // DFSM_H
