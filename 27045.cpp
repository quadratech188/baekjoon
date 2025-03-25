#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include "modules/ListGraph.h"
#include "modules/Types.h"

int main() {
	int n, m;
	std::cin >> n >> m;

	ListGraph<int, None> graph(n);

	for (int i = 0; i < m; i++) {
		int a, b;
		std::cin >> a >> b;
		graph.connect(a - 1, b - 1);
	}

	std::stack<int> stack;
	std::vector<int> parent(n, -1);

	for (int i = 0; i < n; i++) {
		stack.push(i);

		while (!stack.empty()) {
			int parent = stack.top();
			stack.pop();

			if (i == parent[i]) break;

			for (auto child: graph.children(parent))
		}
	}
}
