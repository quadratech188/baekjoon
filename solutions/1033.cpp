#include "../modules/FastIO2.h"
#include "../modules/ListGraph2.h"
#include "../modules/TreeWrapper.h"
#include "../modules/frac.h"
#include <numeric>

template <Graph G>
void dfs(G& graph, size_t parent) {
	for (auto& child: graph.children(parent)) {
		graph[child] = graph[parent] * child.edge();
		dfs(graph, child);
	}
}

int main() {
	size_t n;
	Fast::cin >> n;

	ListGraph<frac<int>, frac<int>> graph(n);

	for (size_t i = 0; i < n - 1; i++) {
		size_t a, b;
		int p, q;
		Fast::cin >> a >> b >> p >> q;

		graph.connect(a, b, {q, p});
		graph.connect(b, a, {p, q});
	}

	TreeWrapper tree(graph, 0);
	tree[0] = {1, 1};

	dfs(tree, 0);

	int lcm = 1;
	for (size_t i = 0; i < graph.size(); i++)
		lcm = std::lcm(lcm, graph[i].bottom());

	for (size_t i = 0; i < graph.size(); i++)
		std::cout << graph[i].to_T(lcm) << ' ';
}
