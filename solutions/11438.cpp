#include "../modules/FastIO2.h"
#include "../modules/SparseTable.h"
#include "../modules/ListGraph2.h"
#include "../modules/Graph.h"
#include "../modules/TreeWrapper.h"
#include <variant>

template <Graph G>
void calc_distance(G& graph, uint parent) {
	for (auto const& child: graph.children(parent)) {
		graph[child] = graph[parent] + 1;
		calc_distance(graph, child);
	}
}

int main() {
	uint n;
	Fast::cin >> n;

	ListGraph<int, std::monostate> graph(n);

	for (uint i = 0; i < n - 1; i++) {
		uint a, b;
		Fast::cin >> a >> b;

		graph.connect(a - 1, b - 1);
		graph.connect(b - 1, a - 1);
	}

	TreeWrapper tree(graph, 0);

	tree[0] = 0;
	calc_distance(tree, 0);

	SparseTable table(n, n - 1, [&tree](uint i) {return tree.parent(i);});

	uint m;
	Fast::cin >> m;

	for (uint i = 0; i < m; i++) {
		uint a, b;
		Fast::cin >> a >> b;
		a --;
		b --;

		if (tree[a] > tree[b])
			a = table.after(a, tree[a] - tree[b]);
		else
		 	b = table.after(b, tree[b] - tree[a]);

		std::cout << table.follow_until_same(a, b) + 1 << '\n';
	}
}
