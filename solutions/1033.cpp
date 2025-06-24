#include "../modules/FastIO2.h"
#include "../modules/ListGraph2.h"
#include "../modules/TreeWrapper.h"
#include <numeric>

template <Graph G>
void dfs(G& graph, size_t parent) {
	for (auto& child: graph.children(parent)) {
		std::pair<int, int> frac = {child.edge().first * graph[parent].first, child.edge().second * graph[parent].second};
		int gcd = std::gcd(frac.first, frac.second);
		graph[child] = {frac.first / gcd, frac.second / gcd};
		dfs(graph, child);
	}
}

int main() {
	size_t n;
	Fast::cin >> n;

	ListGraph<std::pair<int, int>, std::pair<int, int>> graph(n);

	for (size_t i = 0; i < n - 1; i++) {
		size_t a, b;
		int p, q;
		Fast::cin >> a >> b >> p >> q;

		graph.connect(a, b, {p, q});
		graph.connect(b, a, {q, p});
	}

	TreeWrapper tree(graph, 0);
	tree[0] = {1, 1};

	dfs(tree, 0);

	int lcm = 1;
	for (size_t i = 0; i < graph.size(); i++)
		lcm = std::lcm(lcm, graph[i].first);

	for (size_t i = 0; i < graph.size(); i++) {
		std::cout << graph[i].second * (lcm / graph[i].first) << ' ';
	}
}
