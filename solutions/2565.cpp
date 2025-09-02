#include "FastIO2.h"
#include "Matrix.h"
#include <algorithm>
#include <utility>

std::vector<std::pair<int, int>> lines;
Matrix<int> cache;

int solve(uint ptr, int lower_bound) {
	if (ptr == lines.size()) return 0;

	int& result = cache(ptr, lower_bound);

	if (result != -1)
		return result;

	result = solve(ptr + 1, lower_bound);

	if (lines[ptr].second > lower_bound)
		result = std::max(result, 1 + solve(ptr + 1, lines[ptr].second));

	return result;
}

int main() {
	uint n;
	Fast::cin >> n;

	cache = Matrix<int>(n, 501, -1);

	lines = Fast::cin.to_vec<std::pair<int, int>>(n);
	std::ranges::sort(lines);

	std::cout << n - solve(0, 0);
}
