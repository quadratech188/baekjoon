#include "modules/ListGraph.h"
#include "modules/Types.h"
#include "modules/GraphAlgs.h"
#include "modules/FastIO.h"
#include <queue>
#include <iostream>
#include <variant>

void loop() {
	FastIO();
	int n, k;
	std::cin >> n >> k;

	ListGraph<int, std::monostate> graph(n);

	std::vector<int> times(n);

	for (int i = 0; i < n; i++) {
		std::cin >> times[i];
	}

	for (int i = 0; i < k; i++) {
		int a, b;
		std::cin >> a >> b;
		graph.connect(a - 1, b - 1);
	}

	std::vector<int> dependencies = GraphAlgs::inDegree(graph);

	std::queue<std::pair<int, int>> queue;

	for (int i = 0; i < n; i++) {
		if (dependencies[i] == 0) {
			queue.emplace(i, times[i]);
			graph[i] = times[i];
		}
	}

	while (!queue.empty()) {
		int parent = queue.front().first;
		int time = queue.front().second;
		queue.pop();

		for (int child: graph.children(parent)) {
			dependencies[child] --;
			graph[child] = std::max(graph[child], time + times[child]);
			if (dependencies[child] == 0) {
				queue.emplace(child, graph[child]);
			}
		}
	}

	int w;
	std::cin >> w;
	std::cout << graph[w - 1] << '\n';
}

int main() {
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++)
		loop();
}
