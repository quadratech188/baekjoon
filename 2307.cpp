#include "modules/MatrixGraph.h"
#include "modules/FastIO.h"
#include <ctime>
#include <iostream>
#include <limits>
#include <queue>
#include <variant>
int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;

	MatrixGraph<std::monostate, int> graph(n);

	for (int i = 0; i < m; i++) {
		int a, b, t;
		std::cin >> a >> b >> t;
		graph.edge(a - 1, b - 1) = t;
		graph.edge(b - 1, a - 1) = t;
	}

	std::priority_queue<std::pair<int, int>> queue;
	std::vector<int> prev(n, -1);
	std::vector<int> times(n, std::numeric_limits<int>::max());

	queue.emplace(0, 0);
	times[0] = 0;

	while (!queue.empty()) {
		auto [time, parent] = queue.top();
		queue.pop();
		time *= -1;

		if (times[parent] < time) continue;

		if (parent == n - 1) break;

		for (auto child: graph.children(parent)) {
			if (time + child.edge() >= times[child]) continue;

			times[child] = time + child.edge();
			prev[child] = parent;
			queue.emplace(-times[child], child);
		}
	}

	if (prev[n - 1] == -1) {
		std::cout << -1;
		return 0;
	}

	int original = times[n - 1];

	std::vector<std::pair<int, int>> edges;
	for (int i = n - 1; i != 0; i = prev[i]) {
		edges.emplace_back(i, prev[i]);
	}

	int result = 0;

	for (auto edge: edges) {
		int temp = graph.edge(edge.first, edge.second);
		graph.edge(edge.first, edge.second) = 0;
		graph.edge(edge.second, edge.first) = 0;

		std::priority_queue<std::pair<int, int>> queue;
		std::vector<int> times(n, std::numeric_limits<int>::max());

		queue.emplace(0, 0);
		times[0] = 0;

		while (!queue.empty()) {
			auto [time, parent] = queue.top();
			queue.pop();
			time *= -1;

			if (times[parent] < time) continue;

			if (parent == n - 1) break;

			for (auto child: graph.children(parent)) {
				if (times[child] <= time + child.edge()) continue;

				times[child] = time + child.edge();
				queue.emplace(-times[child], child);
			}
		}

		result = std::max(result, times[n - 1]);

		graph.edge(edge.first, edge.second) = temp;
		graph.edge(edge.second, edge.first) = temp;
	}

	std::cout << (result == std::numeric_limits<int>::max()? -1: result - original);
}
