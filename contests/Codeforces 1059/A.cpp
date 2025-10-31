#include "CodeForces.h"
#include "FastIO2.h"
#include <algorithm>
#include <iostream>

int main() {
	uint t;
	Fast::cin >> t;

	for (uint i = 0; i < t; i++) {
		uint n;
		Fast::cin >> n;

		std::cout << std::ranges::max(Fast::cin.to_range<int>(n)) << '\n';
	}
}
