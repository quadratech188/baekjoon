#include "CodeForces.h"
#include "FastIO2.h"
#include <iostream>

/*
 *
 * a b c d d c b a
 *
 * a b c 0 c b a
 */

bool get_bit(uint64_t n, uint i) {
	return (n >> i) & 1;
}

bool solve(uint64_t n) {
	// even
	for (uint i = 1; i <= 32; i ++) {
		if (n >> (2 * i) != 0) continue;
		bool valid = true;
		for (uint k = 0; k < i; k++) {
			if (get_bit(n, k) != get_bit(n, 2 * i - 1 - k)) {
				valid = false;
			}
		}
		if (valid) {
			return true;
		}
	}
	// odd
	for (uint i = 1; i < 32; i ++) {
		if (n >> (2 * i + 1) != 0) continue;
		if (get_bit(n, i) != 0) continue;

		bool valid = true;
		for (uint k = 0; k < i; k++) {
			if (get_bit(n, k) != get_bit(n, 2 * i - k)) {
				valid = false;
			}
		}
		if (valid) {
			return true;
		}
	}
	return false;
}

void loop() {
	uint64_t n;
	Fast::cin >> n;

	std::cout << (solve(n)? "YES": "NO") << '\n';
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
