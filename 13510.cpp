#include "modules/SegTree.h"
#include <iostream>
#include <vector>

struct Edge {
	int start;
	int end;
	int cost;
};

int main() {
	int n;
	std::cin >> n;

	std::vector<Edge> edges;

	edges.reserve(n);

	for (int i = 0; i < n; i++) {
		int u, v, w;
		std::cin >> u >> v >> w;

		edges.push_back({
				.start = u,
				.end = v,
				.cost = w
				});
	}
}
