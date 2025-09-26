#include "CodeForces.h"
#include "FastIO2.h"
#include <iostream>

void loop() {
	int n;
	Fast::cin >> n;

	std::cout << n << ' ';

	for (int i = n - 1; i >= 1; i--)
		std::cout << i << ' ';

	std::cout << n << ' ';

	for (int i = 1; i <= n - 1; i++)
		std::cout << i << ' ';

	std::cout << '\n';
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
