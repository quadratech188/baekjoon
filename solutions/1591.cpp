#include "modules/MatrixGraph.h"
#include "modules/Types.h"
#include <iostream>

int main() {
	int n, m;
	std::cin >> n >> m;

	MatrixGraph<None, int> graph(n);

	for (int i = 0; i < n - m + 1; i++) {
		int prev, current;
		std::cin >> prev;
		for (int j = 1; j < m; j++) {
			std::cin >> current;
			graph.edge(prev, current) = 1;
			prev = current;
		}
	}
}
