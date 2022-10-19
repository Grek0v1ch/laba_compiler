#include "DFSM.h"

DFSM::state DFSM::function_step(const state from, const event curr_event) const {
	if (curr_event == event_error) {
		return from;
	}
	return _table[from][curr_event];
}

DFSM::event DFSM::get_event(const char symbol) const {
	if (isalpha(symbol)) {
		return event_letter;
	}
	if (isdigit(symbol)) {
		return event_digit;
	}
	if (symbol == '"') {
		return event_quotes;
	}
	return event_error;
}

DFSM::DFSM() : _table(table(COUNT_STATE, std::vector<state>(COUNT_EVENT))) {
	_table[st_0_start][event_letter] = st_1_end_id;
	_table[st_0_start][event_digit] = st_2_end_num;
	_table[st_0_start][event_quotes] = st_3;

	_table[st_1_end_id][event_letter] = st_1_end_id;
	_table[st_1_end_id][event_digit] = st_4;
	_table[st_1_end_id][event_quotes] = st_4;

	_table[st_2_end_num][event_letter] = st_4;
	_table[st_2_end_num][event_digit] = st_2_end_num;
	_table[st_2_end_num][event_quotes] = st_4;

	_table[st_3][event_letter] = st_3;
	_table[st_3][event_digit] = st_3;
	_table[st_3][event_quotes] = st_5_end_string;

	_table[st_4][event_letter] = st_4;
	_table[st_4][event_digit] = st_4;
	_table[st_4][event_quotes] = st_4;

	_table[st_5_end_string][event_letter] = st_4;
	_table[st_5_end_string][event_digit] = st_4;
	_table[st_5_end_string][event_quotes] = st_4;
}

type_lex DFSM::process(const std::string lex) const {
	state curr_state = st_0_start;
	for (char symbol : lex) {
		event curr_event = get_event(symbol);
		if (curr_event == event_error) {
			return ERROR;
		}
		curr_state = function_step(curr_state, curr_event);
	}
	switch (curr_state) {
		case st_1_end_id:
			return ID;
		case st_2_end_num:
			return NUM;
		case st_5_end_string:
			return STRING;
		default:
			return ERROR;
	}
}