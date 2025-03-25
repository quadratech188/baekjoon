#include "modules/ListGraph.h"
#include "modules/Types.h"
#include <iostream>
#include <type_traits>

template <typename G>
int dfs(G graph,int parent, std::vector<bool>& visited) {
	visited[parent] = true;
	int result = 1;
	for (auto child: graph.children(parent)) {
		if (visited[child]) continue;
		result += dfs(graph, child, visited);
	}
	return result;
}

int main() {
	int n;
	std::cin >> n;
	if (n == 1) {
		std::cout << 0;
		return 0;
	}

	ListGraph<None, None> graph(n);

	int count = 0;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			char info;
			std::cin >> info;
			if (info == 'Y') {
				graph.connect(i, j);
				count++;
			}
		}
	}

	count /= 2;

	std::vector<bool> visited(n);
	std::vector<int> s;
	for (int i = 0; i < n; i++) {
		if (!visited[i]) {
			visited[i] = true;
			s.push_back(dfs(graph, i, visited));
		}
	}

	int sum = 0;
	for (int a: s) {
		sum += a - 1;
		if (a == 1) {
			std::cout << -1;
			return 0;
		}
	}

	if (s.size() - 1 <= count - sum)
		std::cout << s.size() - 1;
	else
	 	std::cout << -1;
}
