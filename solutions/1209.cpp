#include "FastIO2.h"
#include "Matrix.h"
#include <algorithm>
#include <functional>
#include <ios>
#include <vector>

uint n;
std::vector<int> a, sorted_a;

Matrix<int64_t> cache;

int64_t solve(uint index, uint min) {
	if (index == n)
		return 0;

	int64_t& result = cache(index, min);
	if (result != -1)
		return result;

	result = std::abs(a[index] - sorted_a[min]) + solve(index + 1, min);

	if (min < n - 1)
		result = std::min(result, solve(index, min + 1));

	return result;
}

int main() {
	Fast::cin >> n;
	a = Fast::cin.to_vec<int>(n);
	sorted_a = a;
	std::ranges::sort(sorted_a);

	cache = Matrix<int64_t>(n, n, -1);
	int64_t increasing = solve(0, 0);

	std::ranges::sort(sorted_a, std::greater());

	cache = Matrix<int64_t>(n, n, -1);
	int64_t decreasing = solve(0, 0);

	std::cout << std::min(increasing, decreasing);
}
