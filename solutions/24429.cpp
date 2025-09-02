#include "FastIO2.h"
#include "Matrix.h"
#include <algorithm>

int main() {
	uint n;
	Fast::cin >> n;

	Matrix<int> grid(n, n);

	for (uint i = 0; i < n; i++) {
		for (uint j = 0; j < n; j++) {
			Fast::cin >> grid(i, j);
		}
	}

	uint p;
	Fast::cin >> p;

	auto points = Fast::cin.to_vec<Int2>(p);
	points.emplace_back(n, n);

	std::sort(points.begin(), points.end());

	Matrix<int> cache(n, n);
	cache(0, 0) = grid(0, 0);

	Int2 prev_point(0, 0);
	for (auto& point: points) {
		point -= Int2::one();
		if (point.y < prev_point.y) {
			std::cout << "-1";
			return 0;
		}

		for (int x = prev_point.x + 1; x <= point.x; x++)
			cache(x, prev_point.y) = cache(x - 1, prev_point.y) + grid(x, prev_point.y);
		for (int y = prev_point.y + 1; y <= point.y; y++)
			cache(prev_point.x, y) = cache(prev_point.x, y - 1) + grid(prev_point.x, y);

		for (int x = prev_point.x + 1; x <= point.x; x++) {
			for (int y = prev_point.y + 1; y <= point.y; y++) {
				cache(x, y) = grid(x, y) + std::max(
						cache(x - 1, y),
						cache(x, y - 1)
						);
			}
		}
		prev_point = point;
	}

	std::cout << cache(n - 1, n - 1);
}
