#include "CodeForces.h"
#include "FastIO2.h"
#include "ListGraph2.h"
#include "TreeWrapper.h"
#include "SegmentTree2.h"
#include <iostream>
#include <limits>
#include <variant>

struct Node {
	// max(a, b + x) = b + max(a - b, x)
	int64_t a;
	int64_t b;

	Node operator+(Node const& other) {
		// b + max(a - b, d + max(c - d, x))
		// = b + d + max(a - b - d, max(c - d, x))
		// = b + d + max(max(a - b - d, c - d), x)
		// 
		// f = b + d
		// e - f = max(a - b - d, c - d)
		// e = b + d + max(a - b - d, c - d)
		// = max(a, b + c)
		return {
			std::max(a, b + other.a),
			b + other.b
		};
	}
	// parent: {state, 0}
	// first child: {-infinity, state}
	// last child: {state, -infinity}
	
	int64_t calc(int64_t x) const {
		return std::max(a, b + x);
	}
};

template <Graph G>
uint flatten(G& graph, uint parent, uint counter, std::vector<uint>& out, std::vector<bool>& last_child) {
	out[parent] = counter;
	counter ++;

	uint last_counter;
	for (auto child: graph.children(parent)) {
		last_counter = counter;
		counter = flatten(graph, child, counter, out, last_child);
	}
	last_child[last_counter] = true;
	return counter;
}

void loop() {
	uint n;
	Fast::cin >> n;
	ListGraph<std::monostate, std::monostate> graph(n);

	auto statuses = Fast::cin.to_vec<char>(n);

	for (uint i = 0; i < n - 1; i++) {
		uint u, v;
		Fast::cin >> u >> v;
		graph.connect(u - 1, v - 1);
		graph.connect(v - 1, u - 1);
	}

	TreeWrapper tree(graph, 0);

	std::vector<uint> indices(n);
	std::vector<bool> last_child(n);
	flatten(tree, 0, 0, indices, last_child);

	std::vector<char> flat_statuses(n);
	for (uint i = 0; i < n; i++) {
		flat_statuses[indices[i]] = statuses[i];
	}

	SegmentTree<Node> segtree(std::views::iota(0u, n)
			| std::views::transform([&last_child, &flat_statuses](uint i) {
				return Node {
					flat_statuses[i] - '0',
					last_child[i]? std::numeric_limits<int>::min(): 0
				};
				}));

	std::cout << segtree.root().calc(0) << '\n';
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
