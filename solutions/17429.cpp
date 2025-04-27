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

struct Data {
	unsigned int a;
	unsigned int b;
	unsigned int _value;
	size_t length;

	Data(unsigned int value = 0, size_t length = 1):
		a(1), b(0), _value(value), length(length) {}
	
	constexpr unsigned int value() const {
		return this->a * this->_value + this->b * this->length;
	}

	Data operator+(const Data& other) const {
		return Data(
				value() + other.value(),
				length + other.length
				);
	}

	inline void resolve(Data& child1, Data& child2) {

		child1.a *= a;
		
		child1.b *= a;
		child1.b += b;

		child2.a *= a;
		
		child2.b *= a;
		child2.b += b;

		_value = value();
		a = 1;
		b = 0;
	}
};

int main() {
	FastIO();
	int n, q;
	std::cin >> n >> q;

	ListGraph<std::monostate, std::monostate> graph(n);

	for (int i = 1; i < n; i++) {
		int s, e;
		std::cin >> s >> e;

		graph.connect(s - 1, e - 1);
		graph.connect(e - 1, s - 1);
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

	for (int i = 0; i < q; i++) {
		char type;
		std::cin >> type;

		switch(type) {
			case '1': {
				unsigned int x, v;
				std::cin >> x >> v;
				x--;

				segtree.update(chain_indices[x], chain_indices[x] + subtree_sizes[x], [v](Data& val) {
						val.b += v;
						});
				break;
		  	}
			case '2': {
				unsigned int x, y, v;
				std::cin >> x >> y >> v;
				x--;
				y--;

				while (chain_heads[x] != chain_heads[y]) {
					if (chain_depths[x] < chain_depths[y]) std::swap(x, y);

					segtree.update(
							chain_indices[chain_heads[x]],
							chain_indices[x] + 1,
							[v](Data& val) {val.b += v;});
					x = chain_parents[x];
				}

				if (chain_indices[x] > chain_indices[y])
					std::swap(x, y);

				segtree.update(
						chain_indices[x],
						chain_indices[y] + 1,
						[v](Data& val) {val.b += v;});
				break;
			}
			case '3': {
				unsigned int x, v;
				std::cin >> x >> v;
				x--;

				segtree.update(chain_indices[x], chain_indices[x] + subtree_sizes[x], [v](Data& val) {
						val.a *= v;
						val.b *= v;
						});
				break;
		  	}
			case '4': {
				unsigned int x, y, v;
				std::cin >> x >> y >> v;
				x--;
				y--;

				while (chain_heads[x] != chain_heads[y]) {
					if (chain_depths[x] < chain_depths[y]) std::swap(x, y);

					segtree.update(
							chain_indices[chain_heads[x]],
							chain_indices[x] + 1,
							[v](Data& val) {
							val.a *= v;
							val.b *= v;
							});
					x = chain_parents[x];
				}

				if (chain_indices[x] > chain_indices[y])
					std::swap(x, y);

				segtree.update(
						chain_indices[x],
						chain_indices[y] + 1,
						[v](Data& val) {
						val.a *= v;
						val.b *= v;
						});
				break;
			}
			case '5': {
				unsigned int x;
				std::cin >> x;
				x--;
				std::cout << segtree.sum(chain_indices[x], chain_indices[x] + subtree_sizes[x]).value() << '\n';
				break;
		  	}
			case '6': {
				unsigned int x, y;
				std::cin >> x >> y;
				x--;
				y--;

				unsigned int result = 0;
				while (chain_heads[x] != chain_heads[y]) {
					if (chain_depths[x] < chain_depths[y]) std::swap(x, y);

					result += segtree.sum(
							chain_indices[chain_heads[x]],
							chain_indices[x] + 1
							).value();
					x = chain_parents[x];
				}

				if (chain_indices[x] > chain_indices[y])
					std::swap(x, y);

				result += segtree.sum(
						chain_indices[x],
						chain_indices[y] + 1
						).value();
				std::cout << result << '\n';
				break;
			}
		}
	}
}
