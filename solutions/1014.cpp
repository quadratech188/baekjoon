#include <iostream>
#include "../modules/Matrix.h"
#include "../modules/FastIO.h"

Matrix<int> cache;

Matrix<char> room;

int solve(int prev_state, int row) {
	if (row == room.size().y) return 0;

	int& result = cache(prev_state, row);

	if (result != -1) return result;

	int const max = (1 << room.size().x) - 1;
	int blacklist = max;

	for (int i = 0; i < room.size().x; i++) {
		if (room(i, row) == 'x')
			blacklist &= (max - (1 << i));

		if (prev_state & (1 << i)) {
			if (i > 0)
				blacklist &= (max - (1 << (i - 1)));
			if (i < room.size().x - 1)
				blacklist &= (max - (1 << (i + 1)));
		}
	}

	for (int state = 0; state < (1 << room.size().x); state++) {
		if ((state | blacklist) != blacklist) continue;

		bool eligible = true;
		for (int i = 0; i < room.size().x - 1; i++) {
			if ((state & (1 << i)) && (state & (1 << (i + 1))))
				eligible = false;
		}
		if (!eligible) continue;

		int count = 0;
		for (int i = 0; i < room.size().x; i++) {
			if (state & (1 << i))
				count++;
		}

		result = std::max(result, count + solve(state, row + 1));
	}

	return result;
}

void loop() {
	int n, m;
	std::cin >> n >> m;

	room = Matrix<char>(m, n);

	cache = Matrix<int>(1 << m, n, -1);

	std::cin >> room;

	std::cout << solve(0, 0) << '\n';
}

int main() {
	FastIO();
	int c;
	std::cin >> c;
	for (int i = 0; i < c; i++) loop();
}
