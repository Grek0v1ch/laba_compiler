#ifndef DFSM_H
#define DFSM_H

#include <vector>
#include <string>
#include "token.h"

// Класс детерминированного конечного автомата для распознавания лексем языка.
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

	#define COUNT_STATE 6
	#define COUNT_EVENT 3

	table _table;

	// Метод совершает "шаг" автомата - из состояния по символу переходит в новое состояние.
	state function_step(state, event) const;
	// Метод определяет тип символа.
	event get_event(char) const;

public:
	DFSM();
	// Метод непосредственно моделирует работу автомата.
	type_lexeme process(std::string) const;
};

#endif // DFSM_H
