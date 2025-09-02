#include "FastIO2.h"
#include <algorithm>
#include <iostream>

int main() {
	uint n;
	int m;
	Fast::cin >> n >> m;

	auto cards = Fast::cin.to_vec<int>(n);

	int max = 0;

	for (size_t i = 0; i < n; i++) {
		for (size_t j = i + 1; j < n; j++) {
			for (size_t k = j + 1; k < n; k++) {
				int sum = cards[i] + cards[j] + cards[k];

				if (sum > m) continue;
				max = std::max(max, sum);
			}
		}
	}

	std::cout << max;
}
