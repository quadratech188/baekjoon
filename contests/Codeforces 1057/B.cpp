#include "CodeForces.h"
#include <algorithm>
#include <array>
#include <iostream>
#include "FastIO2.h"

/*
 * a b c | x y z
 * ------|------
 * F F F | F F F
 * F F T | F F F
 * F T F | F F F
 * F T T | F T F
 * T F F | F F F
 * T F T | F F T
 * T T F | T F F
 * T T T | T T T
 *
 */

void loop() {

	std::array<int, 31> counts = {0};

	for (int x: Fast::cin.to_range<int>(3)) {
		for (uint i = 0; i < 31; i++) {
			counts[i] += x & (1 << i)? 1: 0;
		}
	}

	if (std::ranges::any_of(counts, [](int x) {return x == 2;})) {
		std::cout << "NO\n";
	}
	else {
		std::cout << "YES\n";
	}
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
