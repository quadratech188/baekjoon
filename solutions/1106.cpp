#include "../modules/FastIO2.h"
#include <algorithm>
#include <limits>
#include <ranges>
#include <vector>

std::vector<int> cache;

std::vector<std::pair<int, int>> options;

int solve(int target) {
	if (target <= 0) return 0;

	if (cache[target] != -1)
		return cache[target];

	cache[target] = std::ranges::min(options | std::views::transform([target](auto option) {
				return option.first + solve(target - option.second);
				}));

	return cache[target];
}

int main() {
	int c, n;
	Fast::cin >> c >> n;

	cache.resize(c + 1, -1);

	options.resize(n);
	for (auto& val: options)
		Fast::cin >> val.first >> val.second;

	std::cout << solve(c);
}
