#include "../modules/FastIO2.h"
#include <algorithm>
#include <cstdint>
#include <queue>

int main() {
	uint n, k;
	Fast::cin >> n >> k;

	auto jewels = Fast::cin.to_vec<std::pair<int, int>>(n);
	std::ranges::sort(jewels);

	auto bags = Fast::cin.to_vec<int>(k);
	std::ranges::sort(bags);

	std::priority_queue<int> queue;

	int64_t result = 0;

	uint ptr = 0;
	for (auto const& bag: bags) {
		for (; ptr < n; ptr++) {
			if (jewels[ptr].first > bag) break;
			queue.push(jewels[ptr].second);
		}

		if (!queue.empty()) {
			result += queue.top();
			queue.pop();
		}
	}

	std::cout << result;
}
