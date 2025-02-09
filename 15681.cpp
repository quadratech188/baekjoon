#include "modules/ListGraph.h"
#include "modules/TreeWrapper.h"
#include "modules/Types.h"
#include "modules/FastIO.h"
#include <iostream>

std::vector<int> cache;

template <typename G>
int calc(G& graph, int parent) {
	for (auto it: graph.children(parent)) {
		cache[parent] += calc(graph, it);
	}

	return cache[parent];
}

int main() {
	FastIO();
	int n, r, q;
	std::cin >> n >> r >> q;
	ListGraph<None, None> graph(n);
	cache = std::vector<int>(n, 1);

	for (int i = 0; i < n - 1; i++) {
		int u, v;
		std::cin >> u >> v;
		graph.connect(u - 1, v - 1);
		graph.connect(v - 1, u - 1);
	}

	TreeWrapper tree(graph, r - 1);

	calc(tree, r - 1);

	for (int i = 0; i < q; i++) {
		int temp;
		std::cin >> temp;
		std::cout << cache[temp - 1] << '\n';
	}
}
