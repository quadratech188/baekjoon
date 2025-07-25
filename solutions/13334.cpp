#include "../modules/FastIO2.h"
#include <algorithm>

int main() {
	uint n;
	Fast::cin >> n;

	auto edges = Fast::cin.to_vec<std::pair<int, int>>(n);

	int d;
	Fast::cin >> d;

	std::vector<std::pair<int, char>> updates;

	for (auto [h, o]: edges) {
		if (h > o)
			std::swap(h, o);
		if (o - h > d) continue;

		updates.emplace_back(o, 1);
		updates.emplace_back(h + d + 1, -1);
	}

	std::ranges::sort(updates);

	int current = 0;
	int max = 0;

	int last_pos;
	for (auto [pos, update]: updates) {
		// std::cout << pos << ' ' << (int)update << '\n';
		if (last_pos != pos)
			max = std::max(max, current);
		current += update;

		last_pos = pos;
	}

	std::cout << max;
}
