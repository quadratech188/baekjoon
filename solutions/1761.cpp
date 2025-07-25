#include "../modules/ListGraph2.h"
#include "../modules/TreeWrapper.h"
#include "../modules/FastIO2.h"
#include "../modules/LCA.h"
#include <variant>

int main() {
	uint n;
	Fast::cin >> n;

	ListGraph<std::monostate, int> graph(n);

	for (uint i = 0; i < n - 1; i++) {
		uint a, b, d;
		Fast::cin >> a >> b >> d;
		graph.connect(a - 1, b - 1, d);
		graph.connect(b - 1, a - 1, d);
	}

	TreeWrapper tree(graph, 0);

	TreeDistance distances(tree);

	LCA lca(tree);

	uint m;
	Fast::cin >> m;

	for (uint i = 0; i < m; i++) {
		uint a, b;
		Fast::cin >> a >> b;
		a --;
		b --;

		std::cout << distances[a] + distances[b] - 2 * distances[lca.lca(a, b)] << '\n';
	}
}
