#include "modules/ListGraph.h"
#include <iostream>
int main() {
	int n, m;
	std::cin >> n >> m;

	ListGraph<int, int> graph(n);

	for (int i = 0; i < m; i++) {
		int u, v, w;
		std::cin >> u >> v >> w;
		graph.connect(u - 1, v - 1, w);
	}
}
