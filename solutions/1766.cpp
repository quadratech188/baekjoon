#include "../modules/ListGraph2.h"
#include "../modules/FastIO.h"
#include <queue>
#include <iostream>
#include <variant>

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;

	ListGraph<std::monostate, std::monostate> graph(n);

	for (int i = 0; i < m; i++) {
		int a, b;
		std::cin >> a >> b;
		graph.connect(a - 1, b - 1);
	}

	std::vector<int> dependencies(n);

	for (int parent = 0; parent < n; parent++) {
		for (auto const& child: graph.children(parent))
			dependencies[child]++;
	}

	std::priority_queue<int> queue;

	for (int i = 0; i < n; i++) {
		if (dependencies[i] == 0) {
			queue.push(- i);
		}
	}

	while (!queue.empty()) {
		int parent = - queue.top();
		queue.pop();

		std::cout << parent + 1 << ' ';

		for (auto const& child: graph.children(parent)) {
			dependencies[child]--;
			if (dependencies[child] == 0)
				queue.push(-child);
		}
	}
}
