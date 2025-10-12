#include "CodeForces.h"
#include <algorithm>
#include <iostream>
#include "FastIO2.h"
#include <map>
#include <numeric>

void loop() {
	uint n;
	Fast::cin >> n;

	std::map<int, uint> stick_counts;

	for (int len: Fast::cin.to_range<int>(n)) {
		stick_counts[len] ++;
	}

	std::vector<int> sym_lens, odd_lens;
	for (auto [len, cnt]: stick_counts) {
		for (uint i = 0; i < cnt / 2; i++) {
			sym_lens.push_back(len);
		}
		if (cnt % 2 == 1) {
			odd_lens.push_back(len);
		}
	}

	if (sym_lens.size() == 0) {
		std::cout << "0\n";
		return;
	}

	int64_t sum = std::accumulate(sym_lens.begin(), sym_lens.end(), int64_t(0));

	int64_t result = 0;
	if (sym_lens.size() != 1) {
		result = 2 * sum;
	}

	std::ranges::sort(odd_lens);
	if (odd_lens.size() >= 1) {
		for (int i = odd_lens.size() - 1; i >= 0; i --) {
			if (odd_lens[i] < 2 * sum) {
				result = std::max(result, 2 * sum + odd_lens[i]);
				break;
			}
		}
	}
	if (odd_lens.size() >= 2) {
		for (uint i = odd_lens.size() - 1; i > 0; i --) {
			if (odd_lens[i] - odd_lens[i - 1] < 2 * sum) {
				result = std::max(result, 2 * sum + odd_lens[i] + odd_lens[i - 1]);
				break;
			}
		}
	}
	std::cout << result << '\n';
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
