#include "ModInt.h"
#include <iostream>
#include <string>

int main() {
	std::string s;
	std::cin >> s;

	sm32_1e9_7 sum = 0;
	sm32_1e9_7 pow_2 = 1;
	for (auto ch: s) {
		if (ch == 'O')
			sum += pow_2;
		pow_2 *= 2;
	}
	std::cout << sum;
}
