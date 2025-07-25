#include "../modules/FastIO2.h"
#include "../modules/ListGraph2.h"
#include "../modules/Graph.h"
#include "../modules/TreeWrapper.h"
#include "../modules/Matrix.h"
#include <limits>
#include <variant>

Matrix<int> cache;

template <Graph G>
int solve(G& graph, uint current, bool has_to_be_early_adapter) {
	int& result = cache(current, has_to_be_early_adapter? 1 : 0);

	if (result != std::numeric_limits<int>::max())
		return result;

	int case1 = 1;
	
	for (auto const& child: graph.children(current))
		case1 += solve(graph, child, false);

	result = std::min(result, case1);

	if (!has_to_be_early_adapter) {
		int case2 = 0;

		for (auto const& child: graph.children(current))
			case2 += solve(graph, child, true);

		result = std::min(result, case2);
	}

	return result;
}

int main() {
	uint n;
	Fast::cin >> n;

	ListGraph<std::monostate, std::monostate> graph(n);

	for (uint i = 0; i < n - 1; i++) {
		uint u, v;
		Fast::cin >> u >> v;
		u --;
		v --;

		graph.connect(u, v);
		graph.connect(v, u);
	}

	TreeWrapper tree(graph, 0);

	cache = Matrix<int>(n, 2, std::numeric_limits<int>::max());

	std::cout << solve(tree, 0, false);
}
