#include <ios>
#include <iostream>
#include <utility>
#include <vector>
#include <limits>
#include <algorithm>

using int2 = std::pair<int, int>;

int dist(int2 a, int2 b) {
	return std::abs(a.first - b.first) + std::abs(a.second - b.second);
}

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	std::cout.tie(0);

	int n, k;
	std::cin >> n >> k;

	std::vector<int2> points(n);

	for (auto& point: points)
		std::cin >> point.first >> point.second;

	int result = std::numeric_limits<int>::max();

	if (k == 1) {
		for (int i = 0; i < n; i++) {
			int2 root = points[i];
			
			int max = 0;
			for (int j = 0; j < n; j++) {
				if (j == i) continue;
				max = std::max(max, dist(points[j], root));
			}
			result = std::min(result, max);
		}
	}
	else if (k == 2) {
		for (int i = 0; i < n; i++) {
			int2 root1 = points[i];
			for (int j = i + 1; j < n; j++) {
				int2 root2 = points[j];

				int max = 0;
				for (int x = 0; x < n; x++) {
					if (x == i) continue;
					if (x == j) continue;
					max = std::max(max, std::min(
								dist(points[x], root1),
								dist(points[x], root2)
								));
				}
				result = std::min(result, max);
			}
		}
	}
	else if (k == 3) {
		for (int i = 0; i < n; i++) {
			int2 root1 = points[i];
			for (int j = i + 1; j < n; j++) {
				int2 root2 = points[j];

				for (int k = j + 1; k < n; k++) {
					int2 root3 = points[k];

					int max = 0;
					for (int x = 0; x < n; x++) {
						if (x == i) continue;
						if (x == j) continue;
						if (x == k) continue;
						max = std::max(max, std::min({
									dist(points[x], root1),
									dist(points[x], root2),
									dist(points[x], root3)
									}));
					}
					result = std::min(result, max);
				}

			}
		}
	}

	std::cout << result;
}
