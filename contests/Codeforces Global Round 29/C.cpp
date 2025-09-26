#include "CodeForces.h"
#include "FastIO2.h"
#include <iostream>

enum State {
	WHATEVER,
	MANDATORY_RIGHT,
	MANDATORY_LEFT
};

bool loop() {
	uint n;
	Fast::cin >> n;

	uint prev_index = -2;
	State prev_state = WHATEVER;
	uint index = 0;

	bool valid = true;
	for (auto ch: Fast::cin.to_range<char>(n)) {
		if (ch == '1') {
			index ++;
			continue;
		}

		if (index - prev_index >= 3) {
			if (prev_state == MANDATORY_RIGHT) valid = false;
			prev_state = MANDATORY_RIGHT;
		}
		else if (index - prev_index == 2) {
			if (prev_state == MANDATORY_RIGHT)
				prev_state = MANDATORY_LEFT;

			else if (prev_state == MANDATORY_LEFT)
				prev_state = MANDATORY_RIGHT;
			
			else
			 	prev_state = WHATEVER;
		}
		else {
			prev_state = WHATEVER;
		}

		prev_index = index;
		index ++;
	}

	if (prev_state == MANDATORY_RIGHT && prev_index < n - 1)
		valid = false;

	return valid;
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) {
		std::cout << (loop()? "YES": "NO") << '\n';
	}
}
