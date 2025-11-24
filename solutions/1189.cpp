#include "FastIO2.h"
#include "GridGraph.h"
#include <iostream>

Matrix<char> grid(0, 0);
Matrix<char> visited(0, 0);

int solve(Int2 pos, int remaining) {
	if (remaining == 0) {
		return pos == Int2(0, 0);
	}

	Int2 offsets[4] = {
		Int2(0, 1),
		Int2(1, 0),
		Int2(0, -1),
		Int2(-1, 0)
	};

	int result = 0;
	for (Int2 offset: offsets) {
		Int2 new_pos = pos + offset;
		if (new_pos.x < 0 || grid.size().x <= new_pos.x) continue;
		if (new_pos.y < 0 || grid.size().y <= new_pos.y) continue;

		if (grid[new_pos] == 'T') continue;
		if (visited[new_pos]) continue;

		visited[new_pos] = true;
		result += solve(new_pos, remaining - 1);
		visited[new_pos] = false;
	}
	return result;
}

int main() {
	int r, c;
	int k;
	Fast::cin >> r >> c >> k;
	grid = Matrix<char>(r, c);
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			Fast::cin >> grid(r - 1 - i, j);
		}
	}
	visited = Matrix<char>(r, c, false);

	visited(r - 1, c - 1) = true;
	std::cout << solve(Int2(r - 1, c - 1), k - 1);
}
