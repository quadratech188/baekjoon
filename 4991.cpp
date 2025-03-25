#include "modules/GridGraph.h"
#include "modules/FastIO.h"
#include <iostream>
#include <limits>
#include <queue>

struct Data {
	int distance;
	int bitmask;
	Int2 position;
};

bool loop() {
	FastIO();
	int w, h;
	std::cin >> w >> h;
	if (w == 0 && h == 0) return false;

	int power_of_2 = 1;
	Matrix<int> grid(w, h);
	GridGraph<decltype(grid)> graph(grid);

	std::queue<Data> queue;

	for (Int2 index: grid.bounds()) {
		char temp;
		std::cin >> temp;
		switch (temp) {
			case '*':
				grid[index] = power_of_2;
				power_of_2 *= 2;
				break;
			case 'o':
				queue.push({
						.distance = 0,
						.bitmask = 0,
						.position = index
						});
			default:
				grid[index] = temp;
		}
	}
	
	std::vector<Matrix<int>> times(power_of_2, Matrix<int>(w, h, std::numeric_limits<int>::max()));

	while (!queue.empty()) {
		auto [distance, bitmask, position] = queue.front();
		queue.pop();

		if (bitmask == power_of_2 - 1) {
			std::cout << distance << '\n';
			return true;
		}

		for (auto child: graph.children(position)) {
			if (child.value() == 'x') continue;
			int new_bitmask = bitmask;
			if (child.value() != '.' && child.value() != 'o')
				new_bitmask |= child.value();

			if (times[new_bitmask][child] <= distance + 1) continue;

			times[new_bitmask][child] = distance + 1;

			queue.push({
					.distance = distance + 1,
					.bitmask = new_bitmask,
					.position = child
					});
		}
	}

	std::cout << "-1\n";

	return true;
}

int main() {
	while (loop()){}
}
