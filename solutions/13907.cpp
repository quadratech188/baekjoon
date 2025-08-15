#include "FastIO2.h"
#include "ListGraph2.h"
#include "Matrix.h"
#include <algorithm>
#include <limits>
#include <queue>
#include <ranges>
#include <variant>

int main() {
	uint n, m, k;
	Fast::cin >> n >> m >> k;

	uint start, destination;
	Fast::cin >> start >> destination;
	start --;
	destination --;

	ListGraph<std::monostate, int> graph(n);

	for (uint i = 0; i < m; i++) {
		uint a, b;
		int w;
		Fast::cin >> a >> b >> w;
		graph.connect(a - 1, b - 1, w);
		graph.connect(b - 1, a - 1, w);
	}

	// iterations, cost
	Matrix<int> costs(n, n, std::numeric_limits<int>::max());

	std::priority_queue<std::tuple<int, uint, uint>> queue;

	costs(0, start) = 0;
	queue.emplace(0, 0, start);

	while (!queue.empty()) {
		auto [distance, iterations, parent] = queue.top();
		queue.pop();

		distance = - distance;

		if (costs(iterations, parent) < distance) continue;

		if (iterations == n - 1) continue;

		for (auto child: graph.children(parent)) {
			int candidate = distance + child.edge();
			if (costs(iterations + 1, child) <= candidate) continue;

			costs(iterations + 1, child) = candidate;
			queue.emplace(- candidate, iterations + 1, child);
		}
	}

	int64_t min = std::numeric_limits<int64_t>::max();

	for (uint i = 0; i < n; i++) {
		min = std::min(min, int64_t(costs(i, destination)));
	}
	std::cout << min << '\n';

	int64_t raise = 0;
	for (uint i = 0; i < k; i++) {
		int p;
		Fast::cin >> p;
		raise += p;

		int64_t min = std::numeric_limits<int64_t>::max();

		for (uint i = 0; i < n; i++) {
			min = std::min(min, costs(i, destination) + raise * i);
		}
		std::cout << min << '\n';
	}
}
