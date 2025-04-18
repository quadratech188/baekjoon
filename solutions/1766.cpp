#include "modules/ListGraph.h"
#include "modules/Types.h"
#include "modules/FastIO.h"
#include <queue>
#include <iostream>

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;

	ListGraph<None, None> graph(n);

	for (int i = 0; i < m; i++) {
		int a, b;
		std::cin >> a >> b;
		graph.connect(a - 1, b - 1);
	}

	std::vector<int> dependencies(n);

	for (int i = 0; i < n; i++) {
		graph.forEachChild(i, [&dependencies](int child, None& vertex, None& edge) {
				dependencies[child] ++;
				});
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

		graph.forEachChild(parent, [&dependencies, &queue](int child, None& vertex, None& edge) {
				dependencies[child] --;
				if (dependencies[child] == 0)
					queue.push(- child);
				});
	}
}
