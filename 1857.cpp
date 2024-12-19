#include "modules/Matrix.h"
#include "modules/ListGraph.h"
#include "modules/Vec2.h"
#include "modules/Types.h"
#include <array>
#include <iostream>
#include <limits>
#include <queue>

std::array<Int2, 8> moves {
	Int2(1, 2),
	Int2(2, 1),
	Int2(2, -1),
	Int2(1, -2),
	Int2(-1, -2),
	Int2(-2, -1),
	Int2(-2, 1),
	Int2(-1, 2)
};

void dfs(ListGraph<None, None>& graph, Matrix<int>& grid, Int2 parent, Int2 child) {
	for (Int2 move: moves) {
		Int2 newPos = child + move;
		if (!grid.bounds().contains(newPos)) continue;
		if (newPos == parent) continue;

		graph.connect(grid.rawIndex(parent), grid.rawIndex(child));
		dfs(graph, grid, parent, newPos);
	}
}

int main() {

	int m, n;
	std::cin >> m >> n;
	Matrix<int> grid(n, m);
	std::cin >> grid;

	Int2 start, end;

	for (Int2 index: grid.bounds()) {
		if (grid[index] == 3)
			start = index;

		if (grid[index] == 4)
			end = index;
	}

	ListGraph<None, None> graph(m * n);
	dfs(graph, grid, grid.size(), start);
}
