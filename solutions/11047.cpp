#include "FastIO2.h"
#include <iostream>

int main() {
	uint n;
	int k;
	Fast::cin >> n >> k;

	auto coins = Fast::cin.to_vec<int>(n);

	int total_cnt = 0;
	for (auto coin: coins | std::views::reverse) {
		int cnt = k / coin;
		total_cnt += cnt;
		k -= coin * cnt;
	}

	std::cout << total_cnt;
}
