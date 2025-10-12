#include "CodeForces.h"
#include "FastIO2.h"
#include <iostream>

void loop() {
	uint n;
	Fast::cin >> n;
	std::array<int, 101> cnts = {0};
	for (int x: Fast::cin.to_range<int>(n)) {
		cnts[x] ++;
	}

	for (uint i = 0; i < 101; i++) {
		if (cnts[i] == 0) {
			std::cout << i << '\n';
			return;
		}
	}
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
