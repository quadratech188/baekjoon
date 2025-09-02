#include <algorithm>
#include <iostream>
#include <string>
#include "Matrix.h"

Matrix<int> cache;
std::string a, b;

int solve(uint a_ptr, uint b_ptr) {
	if (a_ptr >= a.size() || b_ptr >= b.size()) return 0;
	int& result = cache(a_ptr, b_ptr);
	if (result != -1)
		return result;

	result = std::max({
			result, 
			solve(a_ptr + 1, b_ptr),
			solve(a_ptr, b_ptr + 1)
			});

	if (a[a_ptr] == b[b_ptr])
		result = std::max(result, 1 + solve(a_ptr + 1, b_ptr + 1));

	return result;
}

int main() {
	std::cin >> a >> b;
	cache = Matrix<int>(a.size(), b.size(), -1);

	std::cout << solve(0, 0);
}
