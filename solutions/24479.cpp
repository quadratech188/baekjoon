#define HACKABLE

#include <algorithm>
#include "../modules/Graph.h"
#include "../modules/ListGraph2.h"
#include "../modules/FastIO2.h"
#include "../modules/FastIO.h"
#include <stack>
#include <variant>

template <Graph G>
void dfs(G& graph, uint parent, uint& counter) {
	for (auto& child: graph.children(parent)) {
		if (graph[child] != 0) continue;

		graph[child] = counter++;
		dfs(graph, child, counter);
	}
}

int main() {
	FastIO();
	uint n, m, r;
	Fast::cin >> n >> m >> r;

	ListGraph<uint, std::monostate>
		::with_index<uint> graph(n);

	graph.reserve_children(4);

	for (uint i = 0; i < m; i++) {
		uint u, v;
		Fast::cin >> u >> v;
		graph.connect(u - 1, v - 1);
		graph.connect(v - 1, u - 1);
	}

	for (auto& list: graph._connections)
		std::ranges::sort(list);

	graph[r - 1] = 1;
	uint counter = 2;
	dfs(graph, r - 1, counter);

	for (uint i = 0; i < n; i++)
		std::cout << graph[i] << '\n';
}
