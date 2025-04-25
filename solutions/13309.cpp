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
	int n, q;
	std::cin >> n >> q;

	ListGraph<std::monostate, int> tree(n);

	for (int i = 1; i < n; i++) {
		int a;
		std::cin >> a;

		tree.connect(a - 1, i, 1);
	}

	subtree_sizes.resize(n);
	heavy_edges.resize(n, -1);
	tree_values.resize(n);
	calculate_subtree_sizes(tree);

	chain_heads.resize(n);
	chain_parents.resize(n);
	chain_indices.resize(n);
	chain_depths.resize(n);
	decompose(tree);

	SegmentTree<int, Min<int>> segtree(n);

	for (int i = 0; i < n; i++) {
		segtree.update(chain_indices[i], [i](int& val) {
				val = tree_values[i];
				});
	}

	for (int i = 0; i < q; i++) {
		int b, c, d;
		std::cin >> b >> c >> d;
		b--;
		c--;

		int result = 1;

		int mut_b = b;
		int mut_c = c;

		while (chain_heads[mut_b] != chain_heads[mut_c]) {
			if (chain_depths[mut_b] < chain_depths[mut_c]) std::swap(mut_b, mut_c);
			result = std::min(result, segtree.sum(
						chain_indices[chain_heads[mut_b]],
						chain_indices[mut_b] + 1
						));
			mut_b = chain_parents[mut_b];
		}

		if (chain_indices[mut_b] > chain_indices[mut_c])
			std::swap(mut_b, mut_c);

		if (mut_b != mut_c)
			result = std::min(result, segtree.sum(
					chain_indices[mut_b] + 1,
					chain_indices[mut_c] + 1
					));

		if (result) std::cout << "YES\n";
		else std::cout << "NO\n";

		if (d == 1)
			segtree.update(chain_indices[result? b: c], [](int& val) {val = 0;});
	}
}
