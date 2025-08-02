#include "../modules/FastIO2.h"
#include "../modules/ModInt.h"
#include <algorithm>

/*
 * 6
 * 1 4 5 5 6 10
 * 1 * (2^0 - 2^5) + 4 * (2^1 - 2^4) + 5 * (2^2 - 2^3) + 5 * (2^3 - 2^2) + 6 * (2^4 - 2^1) + 10 * (2^5 - 2^0)
 */

int main() {
	uint n;
	Fast::cin >> n;

	auto values = Fast::cin.to_vec<uint>(n);
	std::ranges::sort(values);

	std::vector<sm32_1e9_7> lookup(n);
	lookup[0] = 1;
	for (uint i = 1; i < n; i++)
		lookup[i] = lookup[i - 1] * 2;

	sm32_1e9_7 sum = 0;
	for (uint i = 0; i < n; i++)
		sum += (lookup[i] - lookup[n - 1 - i]) * values[i];

	std::cout << sum;
}
