#include "FastIO2.h"
#include "Matrix.h"
#include <algorithm>
#include <utility>

int main() {
	uint n, k;
	Fast::cin >> n >> k;

	auto objects = Fast::cin.to_vec<std::pair<uint, int>>(n);

	Matrix<int> cache(n + 1, k + 1);

	for (uint i = 0; i < n; i++) {
		for (uint j = 0; j < objects[i].first; j++) {
			cache(i + 1, j) = cache(i, j);
		}
		for (uint j = objects[i].first; j <= k; j++) {
			cache(i + 1, j) = std::max(
					cache(i, j),
					cache(i, j - objects[i].first) + objects[i].second
					);
		}
	}

	std::cout << cache(n, k);
}
