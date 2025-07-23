#include "../modules/FastIO2.h"
#include <algorithm>
#include <cstdint>

int main() {
	int64_t t;
	Fast::cin >> t;
	uint n;
	Fast::cin >> n;
	auto as = Fast::cin.to_vec<int>(n);

	std::vector<int64_t> a_sums;
	a_sums.reserve(n * (n + 1) / 2);

	for (uint i = 0; i < n; i++) {
		int64_t sum = 0;
		for (uint j = i; j < n; j++) {
			sum += as[j];
			a_sums.push_back(sum);
		}
	}
	std::ranges::sort(a_sums);

	uint m;
	Fast::cin >> m;
	auto bs = Fast::cin.to_vec<int>(m);

	std::vector<int64_t> b_sums;
	b_sums.reserve(m * (m + 1) / 2);

	for (uint i = 0; i < m; i++) {
		int64_t sum = 0;
		for (uint j = i; j < m; j++) {
			sum += bs[j];
			b_sums.push_back(sum);
		}
	}
	std::ranges::sort(b_sums);

	int64_t cnt = 0;
	for (auto& val: a_sums) {
		cnt += std::ranges::upper_bound(b_sums, t - val) - std::ranges::lower_bound(b_sums, t - val);
	}

	std::cout << cnt;
}
