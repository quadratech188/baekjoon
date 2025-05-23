#include "../modules/ListGraph.h"
#include "../modules/Graph.h"
#include "../modules/TreeWrapper.h"
#include "../modules/LazySegmentTree.h"
#include "../modules/LazySum.h"
#include "../modules/FastIO.h"
#include <iostream>
#include <utility>
#include <variant>

std::vector<int> subtree_sizes;
std::vector<int> heavy_edges;

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
	int n, m;
	std::cin >> n >> m;

	ListGraph<std::monostate, int> graph(n);

	for (int i = 1; i < n; i++) {
		int a, b;
		std::cin >> a >> b;

		graph.connect(a - 1, b - 1, 1);
		graph.connect(b - 1, a - 1, 1);
	}

	TreeWrapper tree(graph, 0);

	subtree_sizes.resize(n);
	heavy_edges.resize(n, -1);
	calculate_subtree_sizes(tree);

	chain_heads.resize(n);
	chain_parents.resize(n);
	chain_indices.resize(n);
	chain_depths.resize(n);
	decompose(tree);

	LazySegmentTree<LazySum<long long int>> segtree(n);

	for (int i = 0; i < m; i++) {
		char type;
		std::cin >> type;
		switch(type) {
			case 'P': {
				int u, v;
				std::cin >> u >> v;
				u--;
				v--;

				while (chain_heads[u] != chain_heads[v]) {
					if (chain_depths[u] < chain_depths[v]) std::swap(u, v);
					segtree.update(
							chain_indices[chain_heads[u]],
							chain_indices[u] + 1,
							[](LazySum<long long int>& val) {val += 1;});
					u = chain_parents[u];
				}

				if (chain_indices[u] > chain_indices[v])
					std::swap(u, v);

				if (u != v)
					segtree.update(
							chain_indices[u] + 1,
							chain_indices[v] + 1,
							[](LazySum<long long int>& val) {val += 1;});
				break;
			}
			case 'Q': {
				int u, v;
				std::cin >> u >> v;
				u--;
				v--;

				LazySum<long long int> result;

				while (chain_heads[u] != chain_heads[v]) {
					if (chain_depths[u] < chain_depths[v]) std::swap(u, v);
					result = result + segtree.sum(
								chain_indices[chain_heads[u]],
								chain_indices[u] + 1
								);
					u = chain_parents[u];
				}

				if (chain_indices[u] > chain_indices[v])
					std::swap(u, v);

				if (u != v)
					result = result + segtree.sum(
							chain_indices[u] + 1,
							chain_indices[v] + 1
							);

				std::cout << result.value() << '\n';
			}
		}
	}
}
