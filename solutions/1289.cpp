#include "../modules/ListGraph2.h"
#include "../modules/TreeWrapper.h"
#include "../modules/ModInt.h"
#include "../modules/FastIO.h"
#include <iostream>

sm32_1e9_7 traffic = 0;

template <Graph G>
void solve(G& tree, int root) {
	sm32_1e9_7 sum = 0;
	for (auto& child: tree.children(root)) {
		solve(tree, child);
		traffic += tree[child] * child.edge() * sum;
		sum += tree[child] * child.edge();
	}
	traffic += sum;
	tree[root] = sum + 1;
}

int main() {
	FastIO();
	int n;
	std::cin >> n;

	ListGraph<sm32_1e9_7, sm32_1e9_7> graph(n);

	for (int i = 0; i < n - 1; i++) {
		int a, b, w;
		std::cin >> a >> b >> w;
		graph.connect(a - 1, b - 1, w);
		graph.connect(b - 1, a - 1, w);
	}

	TreeWrapper tree(graph, 0);

	solve(tree, 0);

	std::cout << traffic;
}
