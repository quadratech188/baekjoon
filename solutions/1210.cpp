#include "modules/MatrixGraph.h"

int main() {
	int n, m;
	std::cin >> n >> m;
	int start, end;
	std::cin >> start >> end;

	MatrixGraph<int, int> graph(n);

	for (int i = 0; i < n; i++) {
		std::cin >> graph[i];
	}

	for (int i = 0; i < m; i++) {
		int a, b;
		std::cin >> a >> b;
		graph.edge(a - 1, b - 1) = 1;
		graph.edge(b - 1, a - 1) = 1;
	}


}
