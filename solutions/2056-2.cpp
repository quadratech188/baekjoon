#include <cstdio>
#define FASTISTREAM_BUFFER_SIZE 1 << 15
#include "FastIO2.h"
#include <algorithm>

int main() {
	uint n;
	Fast::cin >> n;

	std::vector<int> times(n);

	for (uint i = 0; i < n; i++) {
		int len, cnt;
		Fast::cin >> len >> cnt;
		for (uint j: Fast::cin.to_range<uint>(cnt)) {
			times[i] = std::max(times[i], times[j - 1]);
		}
		times[i] += len;
	}

	printf("%d", std::ranges::max(times));
}
