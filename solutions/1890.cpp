#include "FastIO2.h"
#include "Matrix.h"
#include <iostream>

int main() {
	int n;
	Fast::cin >> n;

	Matrix<int> grid(n, n, Fast::cin.to_range<int>(n * n));
	Matrix<int64_t> solution(n, n, 0);
	solution(0, 0) = 1;

	for (auto [i, delta]: grid.items()) {
		if (delta == 0) continue;

		for (auto offset: {
			Int2(delta, 0),
			Int2(0, delta)
		}) {
			if (!grid.bounds_check(i + offset)) continue;
			solution[i + offset] += solution[i];
		}
	}
	std::cout << solution(n - 1, n - 1) << '\n';
}
