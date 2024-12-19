#include "modules/ListGraph.h"
#include "modules/Types.h"
#include <iostream>

void toTree()

int main() {
	int n, m, Q;
	std::cin >> n >> m >> Q;

	ListGraph<None, int> graph(n);

	graph.reserve(2 * n);

	std::vector<

	for (int i = 0; i < m; i++) {
		int a, b, c;
		std::cin >> a >> b;
		graph.connect(a - 1, b - 1, c);
		graph.connect(b - 1, a - 1, c);
	}
}
