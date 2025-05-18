#include "../modules/ListGraph2.h"
#include "../modules/TreeWrapper.h"
#include "../modules/ModInt.h"
#include "../modules/FastIO2.h"
#include <ostream>
#include <iostream>
#include <unistd.h>

sm32_1e9_7 traffic = 0;
template <Graph G>
void solve(G& tree, uint root) {
	sm32_1e9_7 sum = 0;
	for (auto const& child: tree.children(root)) {
		solve(tree, child);
		auto const contrib = tree[child] * child.edge();
		traffic += contrib * sum;
		sum += contrib;
	}
	traffic += sum;
	tree[root] = sum + 1;
}

int main() {
	uint n;
	Fast::cin >> n;

	ListGraph<sm32_1e9_7, sm32_1e9_7>
		::index<uint> graph(n);

	graph.reserve_children(4);

	for (uint i = 0; i < n - 1; i++) {
		uint a, b, w;
		Fast::cin >> a >> b >> w;
		graph.connect(a - 1, b - 1, sm32_1e9_7::verified(w));
		graph.connect(b - 1, a - 1, sm32_1e9_7::verified(w));
	}

	TreeWrapper tree(graph, 0);

	solve(tree, 0);

	std::cout << traffic << std::flush;
	_exit(0);
}
