#include "../modules/Matrix.h"
#include "../modules/ModInt.h"
#include <cmath>
#include <cstdint>

struct K {};

uint n, k;
std::vector<dm32<K>> values;
std::vector<uint> lengths;

// Available values(bitmask), target remainder
Matrix<int64_t> cache;

dm32<K> power_cache[51];

int64_t solve(uint16_t available_values, dm32<K> current_remainder) {
	int64_t& result = cache(available_values, current_remainder.val());
	if (result != -1)
		return result;

	result = 0;

	for (uint i = 0; i < n; i++) {
		if (!(available_values & (1 << i))) continue;

		result += solve(available_values ^ (1 << i), current_remainder * power_cache[lengths[i]] + values[i]);
	}

	return result;
}

int main() {
	std::cin >> n;
	std::vector<std::string> raw_values(n);
	for (auto& val: raw_values)
		std::cin >> val;

	std::cin >> k;
	dm32<K>::set_mod(k);

	values.resize(n);
	lengths.resize(n);
	for (uint i = 0; i < n; i++) {
		lengths[i] = raw_values[i].size();

		values[i] = 0;
		for (char ch: raw_values[i])
			values[i] = values[i] * 10 + (ch - '0');
	}

	power_cache[0] = 1;
	for (uint i = 1; i <= 50; i++)
		power_cache[i] = power_cache[i - 1] * 10;

	cache = Matrix<int64_t>(1 << n, k, -1);

	// available_values == 0, target_remainder == 0 -> 1
	cache(0, 0) = 1;
	for (uint i = 1; i < k; i++)
		// available_values == 0, target_remainder != 0 -> 0
		cache(0, i) = 0;

	int64_t numerator = solve((1 << n) - 1, 0);

	int64_t denominator = Math::factorial(int64_t(n));

	int64_t gcd = std::gcd(numerator, denominator);

	std::cout << numerator / gcd << '/' << denominator / gcd << '\n';
}
