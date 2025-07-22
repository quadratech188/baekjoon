#include "../modules/FastIO2.h"
#include <algorithm>
#include <limits>

int lookup[5][5] = {
	{1, 2, 2, 2, 2},
	{2, 1, 3, 4, 3},
	{2, 3, 1, 3, 4},
	{2, 4, 3, 1, 3},
	{2, 3, 4, 3, 1}
};

int main() {
	std::array<std::array<int, 5>, 5> a, b;
	decltype(a)& current = a;
	decltype(a)& prev = b;

	for (uint i = 0; i < 5; i++) {
		for (uint j = 0; j < 5; j++)
			current[i][j] = std::numeric_limits<int>::max() / 2;
	}
	current[0][0] = 0;

	while (true) {
		uint type;
		Fast::cin >> type;

		if (type == 0) break;

		std::swap(current, prev);

		for (uint i = 0; i < 5; i++) {
			for (uint j = 0; j < 5; j++) {
				current[i][j] = std::numeric_limits<int>::max() / 2;

				if (i != type && j != type)
					continue;

				if (i == j)
					continue;

				if (i == type) {
					for (uint k = 0; k < 5; k++)
						current[i][j] = std::min(current[i][j], prev[k][j] + lookup[k][i]);
				}
				if (j == type) {
					for (uint k = 0; k < 5; k++)
						current[i][j] = std::min(current[i][j], prev[i][k] + lookup[k][j]);
				}
			}
		}
	}

	int min = std::numeric_limits<int>::max();

	for (uint i = 0; i < 5; i++) {
		for (uint j = 0; j < 5; j++)
			min = std::min(min, current[i][j]);
	}

	std::cout << min;
}
