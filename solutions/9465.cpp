#include "Matrix.h"
#include "FastIO2.h"
#include <algorithm>

Matrix<int> stickers;
Matrix<int> cache;

void loop() {
	uint n;
	Fast::cin >> n;

	stickers = Matrix<int>(2, n);
	cache = Matrix<int>(3, n, -1);

	for (uint i = 0; i < 2; i++) {
		for (uint j = 0; j < n; j++)
			Fast::cin >> stickers(i, j);
	}

	cache(0, 0) = 0;
	cache(1, 0) = stickers(0, 0);
	cache(2, 0) = stickers(1, 0);
	for (uint i = 1; i < n; i++) {
		cache(0, i) = std::max({
				cache(0, i - 1), cache(1, i - 1), cache(2, i - 1)
				});
		cache(1, i) = std::max(
				cache(0, i - 1), cache(2, i - 1)
				) + stickers(0, i);
		cache(2, i) = std::max(
				cache(0, i - 1), cache(1, i - 1)
				) + stickers(1, i);
	}

	std::cout << std::max({cache(0, n - 1), cache(1, n - 1), cache(2, n - 1)}) << '\n';
}

int main() {
	uint t;
	Fast::cin >> t;

	for (uint i = 0; i < t; i++) loop();
}
