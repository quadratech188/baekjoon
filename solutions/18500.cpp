#include "modules/GridGraph.h"
#include <iostream>
#include <limits>
#include <queue>
#include <ranges>
int main() {
	int r, c;
	std::cin >> r >> c;

	Matrix<char> grid(c, r);
	for (int i = r - 1; i >= 0; i--) {
		for (int j = 0; j < c; j++)
			std::cin >> grid(j, i);
	}
	GridGraph<decltype(grid)> graph(grid);

	int n;
	std::cin >> n;

	for (int i = 0; i < n; i++) {
		int height;
		std::cin >> height;
		height--;
		Int2 hit;
		if (i % 2 == 0) {
			for (int column = 0; column < c; column++)
				if (grid(column, height) == 'x') {
					grid(column, height) = '.';
					break;
				}
		}
		else {
			for (int column = c - 1; column >= 0; column--) {
				if (grid(column, height) == 'x') {
					grid(column, height) = '.';
					break;
				}
			}
		}

		Matrix<char> visited(c, r, false);
		std::queue<Int2> queue;
		for (int column = 0; column < c; column++) {
			if (grid(column, 0) == 'x') {
				visited(column, 0) = true;
				queue.emplace(column, 0);
			}
		}

		while (!queue.empty()) {
			Int2 parent = queue.front();
			queue.pop();
			for (auto child: graph.children(parent)) {
				if (child.value() == '.') continue;
				if (visited[child]) continue;
				visited[child] = true;
				queue.push(child);
			}
		}

		int delta = r;
		for (int column = 0; column < c; column++) {
			int start = 0;
			int end = std::numeric_limits<int>::max();
			for (int row = 0; row < r; row++) {
				if (grid(column, row) == '.') continue;

				if (visited(column, row))
					start = row + 1;
				else
					end = row;

				if (start < end)
					delta = std::min(delta, end - start);
			}
		}

		if (delta == r) continue;

		for (int column = 0; column < c; column++) {
			for (int row = 0; row < r; row++) {
				if (grid(column, row) == '.') continue;
				if (visited(column, row)) continue;
				grid(column, row - delta) = 'x';
				grid(column, row) = '.';
			}
		}
	}

	for (int i = r - 1; i >= 0; i--) {
		for (int j = 0; j < c; j++) {
			std::cout << grid(j, i);
		}
		std::cout << '\n';
	}
}
