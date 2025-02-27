#include "modules/ListGraph.h"
#include "modules/TreeWrapper.h"
#include "modules/Types.h"
#include "modules/SparseTable.h"
#include "modules/FastIO.h"

#include <iostream>

template <typename G>
void calculate_depths_and_weights(G& graph, std::vector<long long int>& weights, int root, int depth = 0, long long int weight = 0) {
	graph[root] = depth;
	weights[root] = weight;

	for (auto child: graph.children(root))
		calculate_depths_and_weights(graph, weights, child, depth + 1, weight + child.edge());
}

template <typename G>
int lca(G& graph, SparseTable& lookup, int a, int b) {
	if (graph[a] > graph[b]) std::swap(a, b);

	b = lookup.after(b, graph[b] - graph[a]);

	return lookup.follow_until_same(a, b);
}

int main() {
	FastIO();
	int n;
	std::cin >> n;

	ListGraph<int, int> graph(n);
	std::vector<long long int> weights(n);

	for (int i = 0; i < n - 1; i++) {
		int u, v, w;
		std::cin >> u >> v >> w;

		graph.connect(u - 1, v - 1, w);
		graph.connect(v - 1, u - 1, w);
	}

	TreeWrapper<decltype(graph)> tree(graph, 0);

	SparseTable lookup(n, n - 1, [&tree](int child) {
			return tree.parent(child);
			});

	calculate_depths_and_weights(tree, weights, 0);

	int m;
	std::cin >> m;
	for (int i = 0; i < m; i++) {
		char type;
		std::cin >> type;
		switch(type) {
			case '1': {
				int u, v;
				std::cin >> u >> v;
				std::cout << weights[u - 1] + weights[v - 1]
					   - 2 * weights[lca(tree, lookup, u - 1, v - 1)] << '\n';
				break;
			}
			case '2': {
				int u, v, k;
				std::cin >> u >> v >> k;
				u -= 1;
				v -= 1;
				k -= 1;

				int ancestor = lca(tree, lookup, u, v);
				int first_half = graph[u] - graph[ancestor];
				if (k <= first_half)
					std::cout << lookup.after(u, k) + 1 << '\n';
				else {
					int length = graph[u] + graph[v]
					       - 2 * graph[ancestor];

					std::cout << lookup.after(v, length - k) + 1 << '\n';
				}
			}
		}
	}
}
