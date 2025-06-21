#include "../../modules/FastIO2.h"
#include <iostream>

void loop() {
	uint n, m;

	if (m < n || m > n * (n + 1) / 2) {
		std::cout << "-1\n";
		return;
	}
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
