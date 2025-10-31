#include "CodeForces.h"
#include "FastIO2.h"
#include <iostream>

/*
 *
 * 0 -> 0
 * 1 -> 1
 * 1 -> 0 trv
 * 0 -> 1:
 * 10110 ^ 01111 ^ 01110
 */

void loop() {
	int a, b;
	Fast::cin >> a >> b;

	std::vector<int> list;

	for (uint i = 0; i < 32; i++) {
		bool a_bit = (a >> i) & 1;
		bool b_bit = (b >> i) & 1;
		if (a_bit == b_bit) continue;
		if (a_bit == 1 && b_bit == 0) {
			list.push_back(1 << i);
			continue;
		}
		// a_bit == 0, b_bit == 1
		bool valid = false;
		for (uint j = i + 1; j < 32; j++) {
			if (a & (1 << j)) {
				list.push_back((1 << j) - (1 << i));
				list.push_back((1 << j) - 2 * (1 << i));
				valid = true;
				break;
			}
		}
		if (!valid) {
			std::cout << "-1\n";
			return;
		}
	}
	std::cout << list.size();
	if (list.size() != 0) std::cout << '\n';
	for (auto x: list) {
		std::cout << x << ' ';
	}
	std::cout << '\n';
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
