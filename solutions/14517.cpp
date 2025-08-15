#include <iostream>
#include <string>
#include "Matrix.h"
#include "ModInt.h"

int main() {
	std::string str;
	std::cin >> str;

	Matrix<sm16_1e4_7> cache(str.size(), str.size() + 1);

	for (uint i = 0; i < str.size(); i++) {
		cache(i, i + 1) = 1;
	}

	for (uint length = 2; length <= str.size(); length++) {
		for (uint l = 0; l <= str.size() - length; l++) {
			uint r = l + length;

			if (str[l] == str[r - 1])
				cache(l, r) = cache(l, r - 1) + cache(l + 1, r) + 1;
			else
				cache(l, r) = cache(l, r - 1) + cache(l + 1, r) - cache(l + 1, r - 1);
		}
	}

	std::cout << cache(0, str.size());
}
