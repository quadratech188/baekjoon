#include "CodeForces.h"
#include "FastIO2.h"
#include <algorithm>
#include <iostream>
#include <set>

void loop() {
	uint n;
	Fast::cin >> n;
	std::array<int, 101> counts {0};
	for (int x: Fast::cin.to_range<int>(n)) {
		counts[x] ++;
	}

	std::cout << 2 * std::ranges::count_if(counts, [](int x) {
			return x != 0;
			}) - 1 << '\n';
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
