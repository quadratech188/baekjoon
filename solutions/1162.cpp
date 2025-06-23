#include "../modules/FastIO2.h"
#include "../modules/ListGraph2.h"
#include "../modules/Matrix.h"
#include <cstdint>
#include <limits>
#include <queue>
#include <variant>

struct QueueItem {
	size_t current_index;
	int64_t current_distance;
	int roads_remaining;

	bool operator<(QueueItem const& other) const {
		if (current_distance != other.current_distance) return current_distance > other.current_distance;
		if (roads_remaining != other.roads_remaining) return roads_remaining < other.roads_remaining;
		return false;
	}
};

int main() {
	int n, m, k;
	Fast::cin >> n >> m >> k;

	ListGraph<std::monostate, int> graph(n);

	for (int i = 0; i < m; i++) {
		int from, to, cost;
		Fast::cin >> from >> to >> cost;
		graph.connect(from - 1, to - 1, cost);
		graph.connect(to - 1, from - 1, cost);
	}

	Matrix<int64_t> min_distances(n, k + 1, std::numeric_limits<int64_t>::max());
	for (int i = 0; i < k; i++)
		min_distances(0, i) = 0;

	std::priority_queue<QueueItem> queue;
	queue.push({0, 0, k});

	while (!queue.empty()) {
		auto [parent, current_distance, roads_remaining] = queue.top();
		queue.pop();

		if (min_distances(parent, roads_remaining) < current_distance) continue;

		if (roads_remaining > 0) {
			for (auto const& child: graph.children(parent)) {
				int64_t dist = current_distance;
				if (min_distances(child, roads_remaining - 1) <= dist) continue;

				for (int i = 0; i <= roads_remaining - 1; i++)
					min_distances(child, i) = std::min(min_distances(child, i), dist);

				queue.push({child, dist, roads_remaining - 1});
			}
		}

		for (auto const& child: graph.children(parent)) {
			int64_t dist = current_distance + child.edge();
			if (min_distances(child, roads_remaining) <= dist) continue;

			for (int i = 0; i <= roads_remaining; i++)
				min_distances(child, i) = std::min(min_distances(child, i), dist);

			queue.push({child, dist, roads_remaining});
		}
	}

	std::cout << min_distances(n - 1, 0);
}
