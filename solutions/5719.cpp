#include "../modules/FastIO2.h"
#include "../modules/ListGraph2.h"
#include "../modules/Matrix.h"
#include <algorithm>
#include <limits>
#include <queue>
#include <variant>

bool loop() {
	uint n, m;
	Fast::cin >> n >> m;

	if (n == 0 && m == 0) return false;

	uint s, d;
	Fast::cin >> s >> d;

	ListGraph<std::monostate, int> graph(n);

	for (uint i = 0; i < m; i++) {
		uint u, v;
		int p;
		Fast::cin >> u >> v >> p;

		graph.connect(u, v, p);
	}

	std::priority_queue<std::pair<int, uint>> queue;

	std::vector<int> distances(n, std::numeric_limits<int>::max());

	std::vector<std::vector<uint>> parents(n);

	distances[s] = 0;
	queue.emplace(0, s);

	while (!queue.empty()) {
		auto [distance, parent] = queue.top();
		queue.pop();
		distance = -distance;

		if (distance > distances[parent]) continue;

		for (auto const& child: graph.children(parent)) {
			if (distance + child.edge() > distances[child]) continue;

			if (distance + child.edge() == distances[child]) {
				parents[child].push_back(parent);
				continue;
			}

			parents[child].clear();
			parents[child].push_back(parent);

			distances[child] = distances[parent] + child.edge();
			queue.emplace(-distances[child], child);
		}
	}

	std::vector<bool> visited(n);
	std::queue<uint> queue2;

	Matrix<char> blacklist(n, n, false);

	visited[d] = true;
	queue2.push(d);

	while (!queue2.empty()) {
		uint child = queue2.front();
		queue2.pop();

		for (auto const& parent: parents[child]) {
			blacklist(parent, child) = true;

			if (visited[parent]) continue;
			visited[parent] = true;

			queue2.push(parent);
		}
	}

	std::ranges::fill(distances, std::numeric_limits<int>::max());

	distances[s] = 0;
	queue.emplace(0, s);

	while (!queue.empty()) {
		auto [distance, parent] = queue.top();
		queue.pop();
		distance = -distance;

		for (auto const& child: graph.children(parent)) {
			if (blacklist(parent, child)) continue;

			if (distance + child.edge() >= distances[child]) continue;

			distances[child] = distance + child.edge();
			queue.emplace(-distances[child], child);
		}
	}

	std::cout << (distances[d] == std::numeric_limits<int>::max()? -1: distances[d]) << '\n';

	return true;
}

int main() {
	while (loop()) {}
}
