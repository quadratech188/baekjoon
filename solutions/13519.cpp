#include "../modules/ListGraph.h"
#include "../modules/Graph.h"
#include "../modules/TreeWrapper.h"
#include "../modules/LazySegmentTree.h"
#include "../modules/FastIO.h"
#include "../modules/Input.h"
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

struct Data {
	std::optional<int> update;

	int _sum_, _sum, sum_, sum;
	int length;

	Data(int val = 0):
		update(), _sum_(val), _sum(val), sum_(val), sum(val), length(1) {}
	Data(int _sum_, int _sum, int sum_, int sum, int length):
		update(), _sum_(_sum_), _sum(_sum), sum_(sum_), sum(sum), length(length) {}

	void fill(int val) {
		update = val;
		sum = val * length;
		if (val > 0) {
			_sum_ = val * length;
			_sum = val * length;
			sum_ = val * length;
		}
		else {
			_sum_ = 0;
			_sum = 0;
			sum_ = 0;
		}
	}

	Data operator+(Data const& other) const {
		return Data(
				std::max(std::max(_sum_, other._sum_), _sum + other.sum_),
				std::max(other._sum, _sum + other.sum),
				std::max(sum_, sum + other.sum_),
				sum + other.sum,
				length + other.length
				);
	}

	void resolve(Data& l, Data& r) {
		if (update.has_value()) {
			l.fill(*update);
			r.fill(*update);
			update.reset();
		}
	}

	Data reverse() {
		return Data(
				_sum_,
				sum_,
				_sum,
				sum,
				length
				);
	}
};

int main() {
	FastIO();
	int n;
	std::cin >> n;

	ListGraph<std::monostate, std::monostate> graph(n);

	auto values = Input::toVec<int>(n);

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

	LazySegmentTree<Data> segtree(n);

	for (int i = 0; i < n; i++) {
		segtree.update(chain_indices[i], [&values, i](Data& val) {
				val.fill(values[i]);
				});
	}

	int m;
	std::cin >> m;
	for (int i = 0; i < m; i++) {
		char type;
		std::cin >> type;
		switch(type) {
			case '1': {
				int u, v;
				std::cin >> u >> v;
				u--;
				v--;

				Data u_sum, v_sum;

				while (chain_heads[u] != chain_heads[v]) {
					if (chain_depths[u] < chain_depths[v]) {
						v_sum = segtree.sum(
								chain_indices[chain_heads[v]],
								chain_indices[v] + 1
								) + v_sum;
						v = chain_parents[v];
					}
					else {
						u_sum = segtree.sum(
								chain_indices[chain_heads[u]],
								chain_indices[u] + 1
								) + u_sum;
						u = chain_parents[u];
					}
				}

				Data result;
				if (chain_indices[u] > chain_indices[v]) {
					result = u_sum.reverse() + segtree.sum(chain_indices[v], chain_indices[u] + 1).reverse() + v_sum;
				}
				else {
					result = u_sum.reverse() + segtree.sum(chain_indices[u], chain_indices[v] + 1) + v_sum;
				}

				std::cout << result._sum_ << '\n';
				break;
			}
			case '2': {
				int u, v, w;
				std::cin >> u >> v >> w;
				u--;
				v--;

				while (chain_heads[u] != chain_heads[v]) {
					if (chain_depths[u] < chain_depths[v]) std::swap(u, v);
					segtree.update(
							chain_indices[chain_heads[u]],
							chain_indices[u] + 1,
							[w](Data& val) {val.fill(w);});
					u = chain_parents[u];
				}

				if (chain_indices[u] > chain_indices[v])
					std::swap(u, v);

				segtree.update(
						chain_indices[u],
						chain_indices[v] + 1,
						[w](Data& val) {val.fill(w);});
				break;
			}
		}
	}
}
