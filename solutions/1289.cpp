#include "../modules/ListGraph2.h"
#include "../modules/TreeWrapper.h"
#include "../modules/ModInt.h"
#include "../modules/FastIO2.h"
#include <iostream>

sm32_1e9_7 traffic = 0;
template <Graph G>
void solve(G& tree, size_t root) {
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
	size_t n;
	Fast::cin >> n;

	ListGraph<sm32_1e9_7, sm32_1e9_7> graph(n);

	for (size_t i = 0; i < n - 1; i++) {
		size_t a, b, w;
		Fast::cin >> a >> b >> w;
		graph.connect(a - 1, b - 1, w);
		graph.connect(b - 1, a - 1, w);
	}

	TreeWrapper tree(graph, 0);

	solve(tree, 0);

	std::cout << traffic;
}
