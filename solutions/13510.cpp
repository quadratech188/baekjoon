#include "../modules/ListGraph.h"
#include "../modules/Graph.h"
#include "../modules/TreeWrapper.h"
#include "../modules/SegmentTree.h"
#include "../modules/Operators.h"
#include "../modules/FastIO.h"
#include <iostream>
#include <utility>
#include <variant>

std::vector<int> subtree_sizes;
std::vector<int> heavy_edges;
std::vector<int> tree_values;

template <Graph G>
int calculate_subtree_sizes(G& tree, int parent = 0) {
	int sum = 1;
	int max = 0;
	for (auto child: tree.children(parent)) {
		int size = calculate_subtree_sizes(tree, child);

		if (size > max) {
			max = size;
			heavy_edges[parent] = child;
		}

		sum += size;

		tree_values[child] = child.edge();
	}

	subtree_sizes[parent] = sum;
	return sum;
}

std::vector<int> chain_heads;
std::vector<int> chain_parents;
std::vector<int> chain_indices;
std::vector<int> chain_depths;

template <Graph G>
void decompose(G& tree, int parent = 0, int depth = 0, int chain_head = 0, int chain_parent = 0) {
	static int index = 0;
	chain_heads[parent] = chain_head;
	chain_parents[parent] = chain_parent;

	chain_indices[parent] = index;
	index++;

	chain_depths[parent] = depth;

	if (heavy_edges[parent] == -1) return;

	decompose(tree, heavy_edges[parent], depth, chain_head, chain_parent);

	for (auto child: tree.children(parent)) {
		if (child != heavy_edges[parent]) {
			decompose(tree, child, depth + 1, child, parent);
		}
	}
}

int main() {
	FastIO();
	int n;
	std::cin >> n;

	ListGraph<std::monostate, int> graph(n);

	std::vector<std::pair<int, int>> edges;
	edges.reserve(n);

	for (int i = 0; i < n - 1; i++) {
		int u, v, w;
		std::cin >> u >> v >> w;

		graph.connect(u - 1, v - 1, w);
		graph.connect(v - 1, u - 1, w);

		edges.emplace_back(u - 1, v - 1);
	}

	TreeWrapper tree(graph, 0);

	subtree_sizes.resize(n);
	heavy_edges.resize(n, -1);
	tree_values.resize(n);
	calculate_subtree_sizes(tree);

	chain_heads.resize(n);
	chain_parents.resize(n);
	chain_indices.resize(n);
	chain_depths.resize(n);
	decompose(tree);

	SegmentTree<int, Max<int>> segtree(n);

	for (int i = 0; i < n; i++) {
		segtree.update(chain_indices[i], [i](int& val) {
				val = tree_values[i];
				});
	}

	int m;
	std::cin >> m;
	for (int _ = 0; _ < m; _++) {
		char type;
		std::cin >> type;
		switch(type) {
			case '1': {
				int i, c;
				std::cin >> i >> c;
				auto [first, second] = edges[i - 1];

				if (tree.parent(first) == second) {
					segtree.update(chain_indices[first], [c](int& val) {
							val = c;
							});
				}
				else {
					segtree.update(chain_indices[second], [c](int& val) {
							val = c;
							});
				}
				break;
		  	}
			case '2': {
				int u, v;
				std::cin >> u >> v;
				u--;
				v--;

				int result = 0;

				while (chain_heads[u] != chain_heads[v]) {
					if (chain_depths[u] < chain_depths[v]) std::swap(u, v);
					result = std::max(result, segtree.sum(
								chain_indices[chain_heads[u]],
								chain_indices[u] + 1
								));
					u = chain_parents[u];
				}

				if (chain_indices[u] > chain_indices[v])
					std::swap(u, v);

				if (u != v)
					result = std::max(result, segtree.sum(
							chain_indices[u] + 1,
							chain_indices[v] + 1
							));

				std::cout << result << '\n';
		  	}
		}
	}
}
