#include "FastIO2.h"
#include <iostream>

int main() {
	int w;
	uint n;
	Fast::cin >> w >> n;

	auto a = Fast::cin.to_vec<int>(n);

	std::vector<std::pair<uint, uint>> founds(w, {0, 0});

	for (uint i = 0; i < n; i++) {
		for (uint j = i + 1; j < n; j++) {
			int weight = a[i] + a[j];
			if (weight >= w) continue;
			founds[weight] = {i, j};
		}
	}

	for (uint i = 0; i < n; i++) {
		for (uint j = i + 1; j < n; j++) {
			int weight = a[i] + a[j];
			if (weight >= w) continue;

			auto [a, b] = founds[w - weight];

			if (a == 0 && b == 0) continue;

			if (i != a && i != b && j != a && j != b) {
				std::cout << "YES\n";
				return 0;
			}
		}
	}

	std::cout << "NO\n";
	return 0;
}
