#include "CodeForces.h"
#include "FastIO2.h"
#include "ListGraph2.h"
#include "TreeWrapper.h"
#include <algorithm>
#include <iostream>
#include <variant>

template <Graph G>
uint dfs(G& tree, uint parent, uint depth) {
	uint size = 0;
	uint sum = 0;
	for (auto child: tree.children(parent)) {
		size ++;
		sum += dfs(tree, child, depth + 1);
	}
	if (size == 0 && depth > 1)
		sum ++;

	return sum;
}

void loop() {
	uint n;
	Fast::cin >> n;

	ListGraph<std::monostate, std::monostate> graph(n);

	for (uint i = 0; i < n - 1; i++) {
		uint u, v;
		Fast::cin >> u >> v;
		graph.connect(u - 1, v - 1);
		graph.connect(v - 1, u - 1);
	}

	uint max_elem = 0;
	for (uint i = 0; i < n; i++) {
		if (graph.degree(max_elem) < graph.degree(i))
			max_elem = i;
	}

	TreeWrapper tree(graph, max_elem);

	std::cout << dfs(tree, max_elem, 1) << '\n';
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
