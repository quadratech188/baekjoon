#include "CodeForces.h"
#include <iostream>
#include "FastIO2.h"
#include <algorithm>

void loop() {
	uint n;
	Fast::cin >> n;

	std::vector<int> counts(n);

	for (auto x: Fast::cin.to_range<int>(n)) {
		counts[x - 1] ++;
	}

	std::cout << std::ranges::count_if(counts, [](int x) {return x != 0;}) << '\n';
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
