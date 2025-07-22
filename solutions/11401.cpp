#include "../modules/ModInt.h"
#include "../modules/Math.h"
#include <iostream>

int main() {
	uint n_raw, k_raw;
	std::cin >> n_raw >> k_raw;

	if (2 * k_raw > n_raw)
		k_raw = n_raw - k_raw;

	sm32_1e9_7 n(n_raw), k(k_raw);

	std::cout << Math::C(n, k);
}
