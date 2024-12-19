#include "modules/ListGraph.h"
#include "modules/Types.h"
#include "modules/FastIO.h"
#include <iostream>

int dfs(std::vector<int>& solution, ListGraph<None, None>& graph, int parent, int root) {
	graph.forEachChild(root, [&solution, parent, root, &graph](int child, None& value, None& edge) {
			if (child == parent) return;
			solution[root] += dfs(solution, graph, root, child);
			});
	return solution[root];
}

int main() {
	FastIO();
	int n, r, q;
	std::cin >> n >> r >> q;
	ListGraph<None, None> graph(n);

	for (int i = 0; i < n - 1; i++) {
		int u, v;
		std::cin >> u >> v;
		graph.connect(u - 1, v - 1);
		graph.connect(v - 1, u - 1);
	}

	std::vector<int> solution(n, 1);
	dfs(solution, graph, r - 1, r - 1);

	for (int i = 0; i < q; i++) {
		int u;
		std::cin >> u;
		std::cout << solution[u - 1] << '\n';
	}
}
