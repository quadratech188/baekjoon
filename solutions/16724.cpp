#include <iostream>
#include "modules/Matrix.h"
#include "modules/UnionFind.h"
#include "modules/FastIO.h"
int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;
	Matrix<char> grid(m, n);
	std::cin >> grid;

	UnionFind uf (m * n);

	for (Int2 index: grid.bounds()) {
		switch(grid[index]) {
			case 'D':
				uf.connect(grid.rawIndex(index), grid.rawIndex(index + Int2(0, 1)));
				break;
			case 'L':
				uf.connect(grid.rawIndex(index), grid.rawIndex(index + Int2(-1, 0)));
				break;
			case 'R':
				uf.connect(grid.rawIndex(index), grid.rawIndex(index + Int2(1, 0)));
				break;
			case 'U':
				uf.connect(grid.rawIndex(index), grid.rawIndex(index + Int2(0, -1)));
				break;
		}
	}
	std::vector<bool>used(m * n);
	for (Int2 index: grid.bounds()) {
		used[uf.find(grid.rawIndex(index))] = true;
	}

	int count = 0;
	for (bool use: used)
		if (use) count++;

	std::cout << count;
}
