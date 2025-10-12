#include <iostream>
#include "CodeForces.h"
#include "FastIO2.h"

void loop() {
	uint n;
	Fast::cin >> n;

	auto b = Fast::cin.to_vec<int64_t>(n);

	std::vector<int> a = {1};
	for (uint i = 1; i < n; i++) {
		/*
		 *
		 * == a[1]:
		 * + (i - 1)
		 * == a[2]:
		 * + (i - 2)
		 * == a[3]:
		 * + (i - 3)
		 * ...
		 * == a[i - 1]:
		 * + 1
		 * new:
		 * + i
		 */

		int diff = b[i] - b[i - 1];
		if (diff <= i) {
			a.push_back(a[i - diff]);
		}
		else {
			a.push_back(i + 1);
		}
	}
	for (auto x: a) {
		std::cout << x << ' ';
	}
	std::cout << '\n';
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
