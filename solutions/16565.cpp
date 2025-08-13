#include "FastIO2.h"
#include "ModInt.h"
#include "Math.h"
#include <iostream>

using sm32_1e4_7 = sm32<10'007>;

int main() {
	uint n;
	Fast::cin >> n;

	sm32_1e4_7 result = 0;

	for (uint number_of_sets = 1; number_of_sets <= n / 4; number_of_sets ++) {
		sm32_1e4_7 delta = Math::C<sm32_1e4_7>(13, number_of_sets)
			* Math::C<sm32_1e4_7>(52 - 4 * number_of_sets, n - 4 * number_of_sets);

		if (number_of_sets % 2 == 1)
			result += delta;
		else
		 	result -= delta;
	}

	std::cout << result;
}
