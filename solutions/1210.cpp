#include "../modules/FastIO.h"
#include "../modules/ListGraph2.h"
#include <cstdint>
#include <iostream>
#include <limits>
#include <queue>
#include <variant>

int main() {
	FastIO();
	size_t n, m;
	std::cin >> n >> m;

	size_t source, sink;
	std::cin >> source >> sink;
	source = 2 * source - 2;
	sink = 2 * sink - 1;

	size_t const size = 2 * n;

	ListGraph<std::monostate, uint64_t>
		::reversible<true> graph(size);

	for (size_t i = 0; i < n; i++) {
		uint64_t cost;
		std::cin >> cost;

		graph.connect_both(2 * i, 2 * i + 1, cost, 0);
	}

	for (size_t i = 0; i < m; i++) {
		size_t a, b;
		std::cin >> a >> b;
		graph.connect_both(2 * a - 1, 2 * b - 2, std::numeric_limits<uint64_t>::max(), 0);
		graph.connect_both(2 * b - 1, 2 * a - 2, std::numeric_limits<uint64_t>::max(), 0);
	}

	std::vector<size_t> prev;
	std::vector<decltype(graph)::child*> edges(size);
	std::queue<size_t> queue;

	while (true) {
		prev.assign(size, std::numeric_limits<size_t>::max());
		queue.push(source);

		while (!queue.empty()) {
			size_t parent = queue.front();
			queue.pop();

			for (auto& child: graph.children(parent)) {
				if (prev[child] == std::numeric_limits<size_t>::max() && child.edge() > 0) {
					prev[child] = parent;
					edges[child] = &child;

					queue.push(child);
				}
			}
		}

		if (prev[sink] == std::numeric_limits<size_t>::max()) break;

		uint64_t max_flow = std::numeric_limits<uint64_t>::max();

		for (size_t ptr = sink; ptr != source; ptr = prev[ptr]) {
			if (edges[ptr]->edge() < max_flow)
				max_flow = edges[ptr]->edge();
		}

		for (size_t ptr = sink; ptr != source; ptr = prev[ptr]) {
			edges[ptr]->edge() -= max_flow;
			graph.reverse(*edges[ptr]).edge() += max_flow;
		}
	}

	std::vector<size_t> answers;
	std::vector<bool> visited(size, false);

	queue.push(source);
	visited[source] = true;

	while (!queue.empty()) {
		size_t parent = queue.front();
		queue.pop();

		for (auto& child: graph.children(parent)) {
			if (!visited[child] && child.edge() > 0) {
				visited[child] = true;
				queue.push(child);
			}
		}
	}
	for (size_t i = 0; i < n; i++) {
		if (visited[2 * i] && !visited[2 * i + 1])
			std::cout << i + 1 << ' ';
	}
}
