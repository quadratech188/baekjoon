#include <iostream>
#include <string>
#include "../modules/Matrix.h"

Matrix<int> cache;
Matrix<std::pair<int, int>> next;
std::string a, b;

int solve(uint a_ptr, uint b_ptr) {
	if (a_ptr == a.size() || b_ptr == b.size())
		return 0;

	if (cache(a_ptr, b_ptr) != -1)
		return cache(a_ptr, b_ptr);

	if (a[a_ptr] == b[b_ptr]) {
		next(a_ptr, b_ptr) = {a_ptr + 1, b_ptr + 1};
		return solve(a_ptr + 1, b_ptr + 1) + 1;
	}

	int l = solve(a_ptr, b_ptr + 1);
	int r = solve(a_ptr + 1, b_ptr);

	int result;
	if (l > r) {
		result = l;
		next(a_ptr, b_ptr) = {a_ptr, b_ptr + 1};
	}
	else {
		result = r;
		next(a_ptr, b_ptr) = {a_ptr + 1, b_ptr};
	}

	cache(a_ptr, b_ptr) = result;

	return result;
}

int main() {
	std::cin >> a >> b;

	cache = Matrix<int>(a.size(), b.size(), -1);
	next = Matrix<std::pair<int, int>>(a.size(), b.size());

	std::cout << solve(0, 0) << '\n';

	uint a_ptr = 0;
	uint b_ptr = 0;

	while (a_ptr != a.size() && b_ptr != b.size()) {
		if (a[a_ptr] == b[b_ptr])
			std::cout << a[a_ptr];

		auto temp = next(a_ptr, b_ptr);
		a_ptr = temp.first;
		b_ptr = temp.second;
	}
}
