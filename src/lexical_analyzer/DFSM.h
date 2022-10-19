#ifndef DFSM_H
#define DFSM_H

#include <vector>
#include <string>

enum type_lex {
	KEYWORD,
	ID,
	NUM,
	SEPARATORS,
	STRING,
	ERROR
};

class DFSM {
private:
	enum state {
		st_0_start,
		st_1_end_id,
		st_2_end_num,
		st_3,
		st_4,
		st_5_end_string
	};

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

	state function_step(const state, const event) const;
	event get_event(const char) const;

public:
	DFSM();
	type_lex process(const std::string) const;
};

#endif // DFSM_H
