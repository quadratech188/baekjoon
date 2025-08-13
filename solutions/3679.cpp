#include "Vec2.h"
#include "FastIO2.h"
#include <algorithm>
#include <iostream>
#include <numeric>

void loop() {
	uint n;
	Fast::cin >> n;

	auto points = Fast::cin.to_vec<Int2>(n);

	std::vector<uint> indices(n);
	std::iota(indices.begin(), indices.end(), 0);

	auto min_element = std::ranges::min_element(indices, [&points](uint l, uint r) {
			return points[l] < points[r];
			});

	std::swap(indices[0], *min_element);

	uint root = indices[0];

	std::sort(indices.begin() + 1, indices.end(), [&points, root](uint l, uint r) {
			int temp = (points[l] - points[root]).cross(points[r] - points[root]);
			if (temp != 0) return temp > 0;
			return (points[l] - points[root]).size2() > (points[r] - points[root]).size2();
			});

	uint after_root = indices[1];
	auto end_of_beginning_edge = std::partition_point(indices.begin() + 1, indices.end(), [&points, root, after_root](int x) {
			return (points[x] - points[root]).cross(points[after_root] - points[root]) == 0;
			});

	std::sort(indices.begin(), end_of_beginning_edge, [&points, root](uint l, uint r) {
			return (points[l] - points[root]).size2() < (points[r] - points[root]).size2();
			});

	for (uint idx: indices)
		std::cout << idx << ' ';

	std::cout << '\n';
}

int main() {
	uint c;
	Fast::cin >> c;
	for (uint i = 0; i < c; i++) loop();
}
