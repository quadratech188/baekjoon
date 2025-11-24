#include "ListGraph2.h"
#include "FastIO2.h"
#include "Graph.h"
#include <variant>
#include <iostream>

template <Graph G>
uint dfs(G& graph, uint parent, uint ignore) {
	if (parent == ignore) return 0;
	uint result = 0;

	for (auto child: graph.children(parent)) {
		result += dfs(graph, child, ignore);
	}
	if (result == 0) result = 1;
	return result;
}

int main() {
	uint n;
	Fast::cin >> n;
	uint root;
	ListGraph<std::monostate, std::monostate> graph(n);

	for (uint i = 0; i < n; i++) {
		int x;
		Fast::cin >> x;
		if (x == -1) {
			root = i;
		}
		else {
			graph.connect(x, i);
		}
	}
	uint del;
	Fast::cin >> del;

	std::cout << dfs(graph, root, del);
}
