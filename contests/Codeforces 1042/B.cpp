#include "CodeForces.h"
#include "FastIO2.h"
#include <iostream>

void loop() {
	uint n;
	Fast::cin >> n;

	if (n % 2 == 0) {
		for (uint i = 0; i < n / 2 - 1; i++)
			std::cout << "-1 3 ";

		std::cout << "-1 2\n";
		return;
	}
	else {
		for (uint i = 0; i < n / 2; i++)
			std::cout << "-1 3 ";

		std::cout << "-1\n";
	}
}

int main() {
	uint t;
	Fast::cin >> t;

	for (uint i = 0; i < t; i++) loop();
}
