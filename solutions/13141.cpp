#include "FastIO2.h"
#include "ListGraph2.h"
#include "Matrix.h"
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <queue>
#include <sys/types.h>
#include <variant>

int main() {
	uint n, m;
	Fast::cin >> n >> m;

	ListGraph<std::monostate, int> graph(n);

	for (uint i = 0; i < m; i++) {
		uint s, e;
		int l;
		Fast::cin >> s >> e >> l;

		graph.connect(s - 1, e - 1, l);
		graph.connect(e - 1, s - 1, l);
	}

	std::vector<int> distances(n);

	std::priority_queue<std::pair<int, uint>> queue;
	double min_duration = std::numeric_limits<double>::max();
	for (uint root = 0; root < n; root ++) {
		std::ranges::fill(distances, std::numeric_limits<int>::max());
		distances[root] = 0;

		queue.emplace(0, root);

		while (!queue.empty()) {
			auto [distance, parent] = queue.top();
			distance = - distance;
			queue.pop();

			if (distances[parent] < distance) continue;

			for (auto child: graph.children(parent)) {
				int candidate = distances[parent] + child.edge();
				if (distances[child] <= candidate) continue;

				distances[child] = candidate;
				queue.emplace(- candidate, child);
			}
		}

		double max_duration = 0;
		for (uint parent = 0; parent < n; parent++) {
			for (auto child: graph.children(parent)) {
				if (std::abs(distances[parent] - distances[child]) >= child.edge()) {
					max_duration = std::max({max_duration, (double)distances[parent], (double)distances[child]});
				}
				else {
					max_duration = std::max(max_duration, (double)(distances[parent] + distances[child] + child.edge()) / 2);
				}
			}
		}
		min_duration = std::min(min_duration, max_duration);
	}

	std::cout << std::fixed << std::setprecision(1) << min_duration;
}
