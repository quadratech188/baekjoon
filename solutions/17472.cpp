#include "modules/Matrix.h"
#include "modules/UnionFind.h"
#include "modules/FastIO.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <queue>

static std::array<Int2, 4> deltas {
	Int2(1, 0),
	Int2(0, 1),
	Int2(-1, 0),
	Int2(0, -1)
};

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;
	Matrix<int> world(m, n);
	std::cin >> world;

	Matrix<int> groups(m, n, -1);
	std::vector<Int2> roots;

	for (Int2 root: world.bounds()) {
		if (world[root] == 0) continue;
		if (groups[root] != -1) continue;

		int index = roots.size();
		roots.push_back(root);

		std::queue<Int2> queue;
		queue.push(root);
		groups[root] = index;

		while (!queue.empty()) {
			Int2 parent = queue.front();
			queue.pop();

			for (Int2 delta: deltas) {
				Int2 child = parent + delta;
				if (!world.bounds().contains(child)) continue;
				if (world[child] == 0) continue;
				if (groups[child] != -1) continue;

				groups[child] = index;
				queue.push(child);
			}
		}
	}

	Matrix<int> distances(roots.size(), roots.size(), std::numeric_limits<int>::max());

	for (Int2 parent: world.bounds()) {
		if (groups[parent] == -1) continue;
		for (Int2 delta: deltas) {
			for (int i = 1;; i++) {
				Int2 child = parent + delta * i;
				if (!world.bounds().contains(child)) break;

				if (groups[child] == groups[parent]) break;

				if (groups[child] != -1) {
					if (i == 2) break;

					int& distance = distances(groups[parent], groups[child]);
					distance = std::min(distance, i - 1);
					break;
				}
			}
		}
	}

	std::vector<std::tuple<int, int, int>> edges;

	for (int i = 0; i < roots.size(); i++) {
		for (int j = i + 1; j < roots.size(); j++) {
			if (distances(i, j) != std::numeric_limits<int>::max())
				edges.emplace_back(distances(i, j), i, j);
		}
	}

	std::sort(edges.begin(), edges.end());

	UnionFind uf(roots.size());

	int total = 0;

	for (auto edge: edges) {
		if (uf.connected(std::get<1>(edge), std::get<2>(edge))) continue;

		uf.connect(std::get<1>(edge), std::get<2>(edge));
		total += std::get<0>(edge);
	}

	int parent = uf.find(0);
	for (int i = 1; i < roots.size(); i++) {
		if (parent != uf.find(i)) {
			std::cout << -1;
			return 0;
		}
	}

	std::cout << total;
}
