#include "../modules/ModInt.h"
#include <iostream>
#include <optional>

using sm32_9901 = sm32<9901>;

std::optional<sm32_9901> cache[1 << 14][14][14];

int n, m;

sm32_9901 solution(int state, int length, int cursor) {
	if (cursor == n)
		return solution(state, length - 1, 0);
	if (length == 0) {
		if (state == 0) return 1;
		return 0;
	}

	if (cache[state][length][cursor].has_value())
		return *cache[state][length][cursor];

	if (cursor == n - 1) {
		if (state & (1 << cursor))
			return solution(state ^ (1 << cursor), length - 1, 0);
		else
			return solution(state | (1 << cursor), length - 1, 0);
	}
	
	sm32_9901 result = 0;

	if (!(state & (1 << cursor)) && !(state & (1 << (cursor + 1))))
		result += solution(state, length, cursor + 2);

	if (!(state & (1 << cursor)))
		result += solution(state | (1 << cursor), length, cursor + 1);
	else
		result += solution(state ^ (1 << cursor), length, cursor + 1);

	cache[state][length][cursor] = result;

	return result;
}

int main() {
	std::cin >> n >> m;

	std::cout << solution(0, m, 0);
}
