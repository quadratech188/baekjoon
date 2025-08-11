#include "FastIO2.h"
#include "Matrix.h"
#include <cstdint>
#include <limits>

uint n;
int w;
Matrix<int> enemies;
Matrix<int> cache;

int solve(uint ptr, uint8_t begin_status, uint8_t ptr_status) {
	if (ptr > n)
		return std::numeric_limits<int>::max() / 2;
	if (ptr == n)
		return begin_status == ptr_status? 0: std::numeric_limits<int>::max() / 2;

	int& result = cache(ptr, 4 * begin_status + ptr_status);
	if (result != -1)
		return result;

	result = std::numeric_limits<int>::max();

	switch(ptr_status) {
		case 0:
			// __ -> _-
			result = std::min(result, 1 + solve(ptr, begin_status, 2));
			// __ -> -_
			result = std::min(result, 1 + solve(ptr, begin_status, 1));

			// __ -> --
			if (enemies(ptr, 0) + enemies(ptr, 1) <= w)
				result = std::min(result, 1 + solve(ptr, begin_status, 3));

			// __ -> ^^
			if (enemies(ptr, 0) + enemies(ptr + 1, 0) <= w
					&& enemies(ptr, 1) + enemies(ptr + 1, 1) <= w)
				result = std::min(result, 2 + solve(ptr + 1, begin_status, 3));

			return result;
		case 1:
			// -_ -> --
			result = std::min(result, 1 + solve(ptr, begin_status, 3));

			// -_ -> -^
			if (enemies(ptr, 1) + enemies(ptr + 1, 1) <= w)
				result = std::min(result, 1 + solve(ptr + 1, begin_status, 2));

			return result;
		case 2:
			// _- -> --
			result = std::min(result, 1 + solve(ptr, begin_status, 3));

			// _- -> ^-
			if (enemies(ptr, 0) + enemies(ptr + 1, 0) <= w)
				result = std::min(result, 1 + solve(ptr + 1, begin_status, 1));

			return result;
		case 3:
			// -- -> --
			result = std::min(result, solve(ptr + 1, begin_status, 0));

			return result;
		default:
			__builtin_unreachable();
	}
}

void loop() {
	Fast::cin >> n >> w;

	enemies = Matrix<int>(n + 1, 2);
	for (uint j = 0; j < 2; j++) {
		for (uint i = 0; i < n; i++)
			Fast::cin >> enemies(i, j);
	}
	enemies(n, 0) = enemies(0, 0);
	enemies(n, 1) = enemies(0, 1);

	cache = Matrix<int>(n, 16, -1);

	int result = std::numeric_limits<int>::max();

	for (uint8_t i = 0; i < 4; i++)
		result = std::min(result, solve(0, i, i));

	std::cout << result << '\n';
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
