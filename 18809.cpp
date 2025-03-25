#include "modules/GridGraph.h"
#include <iostream>
#include <limits>
#include <queue>
int main() {
	int n, m, g, r;
	std::cin >> n >> m >> g >> r;
	Matrix<int> grid(m, n);
	std::cin >> grid;
	GridGraph<decltype(grid)> graph(grid);

	std::vector<Int2> available;
	available.reserve(g + r);
	for (Int2 index: grid.bounds()) {
		if (grid[index] == 2) available.push_back(index);
	}

	std::vector<Matrix<int>> distances(g + r, Matrix<int>(m, n, std::numeric_limits<int>::max()));

	for (int i = 0; i < g + r; i++) {
		Matrix<int>& local_distances = distances[i];
		std::queue<std::pair<int, Int2>> queue;
		queue.emplace(0, available[i]);

		while (!queue.empty()) {
			auto [distance, parent] = queue.front();
			queue.pop();

			for (auto child: graph.children(parent)) {
				if (child.value() == )
			}
		}
	}
}
