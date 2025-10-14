#include "FastIO2.h"
#include <algorithm>
#include <iostream>
#include <ranges>

int main() {
	uint n;
	int64_t k;
	Fast::cin >> n >> k;

	auto values = Fast::cin.to_vec<int64_t>(n);
	std::ranges::sort(values);

	std::vector<int64_t> partial_sums(n + 1);

	for (uint i = 0; i < n; i++) {
		partial_sums[i + 1] = partial_sums[i] + values[i];
	}

	auto range = std::views::iota(size_t(0), size_t(1e12) + 1);

	auto result = std::ranges::partition_point(range, [k, &values, &partial_sums](size_t i) {
			auto divide = std::ranges::upper_bound(values, i);
			int64_t sum = partial_sums.back() - partial_sums[divide - values.begin()];
			return k < sum - (values.end() - divide) * i;
			});

	std::cout << *result;
}
