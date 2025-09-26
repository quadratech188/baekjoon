#include "CodeForces.h"
#include "FastIO2.h"
#include <iostream>

void loop() {
	int x, y;
	Fast::cin >> x >> y;

	int result;

	if (x < y) {
		result = 2;
	}
	else if (y == 1) {
		result = -1;
	}
	else if (x >= y + 2) {
		result = 3;
	}
	else {
		result = -1;
	}

	std::cout << result << '\n';
	return;
}

int main() {
	uint t;
	Fast::cin >> t;

	for (uint i = 0; i < t; i++) loop();
}
