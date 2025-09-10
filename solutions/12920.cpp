#include "FastIO2.h"
#include "Matrix.h"

int main() {
	uint n;
	int m;
	Fast::cin >> n >> m;

	std::vector<std::pair<int, int>> objects;

	for (uint i = 0; i < n; i++) {
		int v, c;
		uint k;
		Fast::cin >> v >> c >> k;

		uint chunk = 1;
		while (true) {
			if (k < chunk) break;

			objects.emplace_back(v * chunk, c * chunk);

			k -= chunk;
			chunk *= 2;
		}
		objects.emplace_back(v * k, c * k);
	}

	uint count = objects.size();
	Matrix<int> cache(count + 1, m + 1);

	for (uint i = 0; i < count; i++) {
		for (int j = 0; j < objects[i].first; j++) {
			cache(i + 1, j) = cache(i, j);
		}
		for (int j = objects[i].first; j <= m; j++) {
			cache(i + 1, j) = std::max(
					cache(i, j),
					cache(i, j - objects[i].first) + objects[i].second
					);
		}
	}

	std::cout << cache(count, m);
}
