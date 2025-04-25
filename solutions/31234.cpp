#include "../modules/ListGraph.h"
#include "../modules/Graph.h"
#include "../modules/TreeWrapper.h"
#include "../modules/LazySegmentTree.h"
#include "../modules/FastIO.h"
#include <algorithm>
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

struct Min {
	int min;
	int delta;

	Min(int value):
		min(value), delta(0) {}
	Min():
		min(std::numeric_limits<int>::max()), delta(0) {}

	void resolve(Min& left, Min& right) {
		left.delta += delta;
		right.delta += delta;
		min += delta;
		delta = 0;
	}

	int value() const {
		return min + delta;
	}

	Min operator+(Min const& other) const {
		return Min(std::min(value(), other.value()));
	}
};

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;

	ListGraph<std::monostate, std::monostate> graph(n);

	for (int i = 1; i < n; i++) {
		int a, b;
		std::cin >> a >> b;

		graph.connect(a - 1, b - 1);
		graph.connect(b - 1, a - 1);
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

	std::vector<int> v(n);
	for (int i = 0; i < n; i++) {
		int a;
		std::cin >> a;

		v[chain_indices[i]] = a;
	}

	LazySegmentTree<Min> segtree(v);

	int max = -1;
	for (int i = 0; i < m; i++) {
		int x, y, w;
		std::cin >> x >> y >> w;
		x--;
		y--;

		if (max != -1) continue;

		int x_mut = x;
		int y_mut = y;
		Min result;

		while (chain_heads[x_mut] != chain_heads[y_mut]) {
			if (chain_depths[x_mut] < chain_depths[y_mut]) std::swap(x_mut, y_mut);
			result = result + segtree.sum(
						chain_indices[chain_heads[x_mut]],
						chain_indices[x_mut] + 1
						);
			x_mut = chain_parents[x_mut];
		}

		if (chain_indices[x_mut] > chain_indices[y_mut])
			std::swap(x_mut, y_mut);

		result = result + segtree.sum(
				chain_indices[x_mut],
				chain_indices[y_mut] + 1
				);

		if (result.value() < w) {
			max = i;
			continue;
		}

		while (chain_heads[x] != chain_heads[y]) {
			if (chain_depths[x] < chain_depths[y]) std::swap(x, y);
			segtree.update(
					chain_indices[chain_heads[x]],
					chain_indices[x] + 1,
					[w](Min& val) {val.delta -= w;});
			x = chain_parents[x];
		}

		if (chain_indices[x] > chain_indices[y])
			std::swap(x, y);

		segtree.update(
				chain_indices[x],
				chain_indices[y] + 1,
				[w](Min& val) {val.delta -= w;});
	}

	std::cout << (max == -1? m: max);
}
