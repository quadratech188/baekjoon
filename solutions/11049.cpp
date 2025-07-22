#include "../modules/FastIO2.h"
#include "../modules/Matrix.h"
#include <algorithm>
#include <limits>

Matrix<int> cache;

std::vector<std::pair<int, int>> matrices;

int solution(uint l, uint r) {
	if (cache(l, r) != -1)
		return cache(l, r);

	if (r - l <= 1)
		return 0;

	int result = std::numeric_limits<int>::max();

	for (uint divider = l + 1; divider < r; divider++) {
		result = std::min(result, solution(l, divider) + solution(divider, r)
				+ matrices[l].first * matrices[divider].first * matrices[r - 1].second);
	}

	cache(l, r) = result;
	return result;
}

int main() {
	uint n;
	Fast::cin >> n;

	matrices = Fast::cin.to_vec<std::pair<int, int>>(n);

	cache = Matrix<int>(n, n + 1, -1);

	std::cout << solution(0, n);
}
