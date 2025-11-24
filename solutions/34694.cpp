#include "FastIO2.h"
#include <algorithm>
#include <iostream>
#include <vector>
uint a, b, w, M;
std::vector<int> flexible_classes;
std::vector<int> max_heights;

int main() {
	Fast::cin >> a >> b >> w >> M;
	flexible_classes = Fast::cin.to_vec<int>(a);
	max_heights.resize(w);
	for (auto& c: max_heights) {
		c = std::ranges::max(Fast::cin.to_vec<int>(b));
	}

	std::ranges::sort(flexible_classes, std::greater{});
	std::ranges::sort(max_heights, std::greater{});

	int64_t result = 0;
	for (uint i = 0; i < w; i++) {
		int max_height = max_heights[i];
		if ((M - b) * i < a) {
			max_height = std::max(max_height, flexible_classes[(M - b) * i]);
		}
		result += 2 * max_height - 2;
	}
	std::cout << result << '\n';
}
