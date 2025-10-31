#include "CodeForces.h"
#include "FastIO2.h"
#include <iostream>
/*
 *
 *
 * 2 n - 1
 * ...
 * 2 3
 * 1 6
 * 1 n
 *
 * -> 2 * (n + 2) (n - 3) / 2 + 6 = n^2
 */

void loop() {
	int n;
	Fast::cin >> n;

	if (n == 2) {
		std::cout << "-1\n";
		return;
	}
	if (n == 3) {
		std::cout << "1 3\n2 3\n";
		return;
	}
	if (n == 4) {
		std::cout << "1 2\n3 1\n4 1\n";
		return;
	}
	if (n == 5) {
		std::cout << "5 1\n4 3\n3 2\n2 1\n";
		return;
	}
	if (n == 6) {
		std::cout << "6 3\n5 2\n4 3\n3 2\n3 1\n";
		return;
	}
	
	for (int i = 3; i <= n - 1; i++) {
		std::cout << "2 " << i << '\n';
	}
	std::cout << "1 6\n";
	std::cout << "1 " << n << '\n';
	return;
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
