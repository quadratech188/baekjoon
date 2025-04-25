#include "../modules/ListGraph.h"
#include "../modules/Graph.h"
#include "../modules/TreeWrapper.h"
#include "../modules/SegmentTree.h"
#include "../modules/Operators.h"
#include "../modules/FastIO.h"
#include "../modules/Input.h"
#include <functional>
#include <iostream>
#include <limits>
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
	int n, q;
	std::cin >> n >> q;

	auto values = Input::toVec<int>(n);

	ListGraph<std::monostate, std::monostate> graph(n);


	for (int i = 1; i < n; i++) {
		int u, v;
		std::cin >> u >> v;

		graph.connect(u - 1, v - 1);
		graph.connect(v - 1, u - 1);
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

	SegmentTree<int, std::bit_xor<int>> segtree(n);

	for (int i = 0; i < n; i++)
		segtree.update(chain_indices[i], [&values, i](int& val) {
				val = values[i];
				});

	for (int _ = 0; _ < q; _++) {
		char type;
		std::cin >> type;
		switch(type) {
			case '1': {
				int i, v;
				std::cin >> i >> v;
				i--;

				segtree.update(chain_indices[i], [v](int& val) {
						val = v;
						});
				break;
		  	}
			case '2': {
				int i, j;
				std::cin >> i >> j;
				i--;
				j--;

				int result = 0;
				while (chain_heads[i] != chain_heads[j]) {
					if (chain_depths[i] < chain_depths[j]) std::swap(i, j);
					result ^= segtree.sum(
								chain_indices[chain_heads[i]],
								chain_indices[i] + 1
								);
					i = chain_parents[i];
				}

				if (chain_indices[i] > chain_indices[j])
					std::swap(i, j);

				result ^= segtree.sum(
						chain_indices[i],
						chain_indices[j] + 1
						);

				std::cout << result << '\n';
			}
		}
	}
}
