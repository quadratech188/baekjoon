#include "FastIO2.h"
#include <bit>
#include <iostream>

int main() {
	uint n;
	int k;
	Fast::cin >> n >> k;

	for (uint t = 0; t < n; t++) {
		if(std::popcount(n + t) <= k) {
			std::cout << t;
			break;
		}
	}
}
