#include "modules/GridGraph.h"
#include "modules/ListGraph.h"
#include "modules/FastIO.h"
#include <iostream>
#include <limits>
#include <queue>

constexpr int lookup[6][6] = {
	100, 70, 40, 0, 0, 0,
	70, 50, 30, 0, 0, 0,
	40, 30, 20, 0, 0, 0,
	0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0
};

struct Edge {
	int cost;
	int capacity;
	int rev;
};

int main() {
	FastIO();
	int n;
	std::cin >> n;

	Matrix<char> matrix(n, n);
	std::cin >> matrix;
	GridGraph<decltype(matrix)> graph(matrix);

	int const source = n * n;
	int const sink = n * n + 1;
	int const size = n * n + 2;
	ListGraph<std::monostate, Edge> flowgraph(size);

	for (Int2 parent: matrix.bounds()) {
		int index = matrix.rawIndex(parent);

		flowgraph.connect_both(index, sink, {0, 1, 0}, {0, 0, 0});

		if ((parent.x + parent.y) % 2 != 0) continue;

		flowgraph.connect_both(source, index, {0, 1, 0}, {0, 0, 0});

		for (auto child: graph.children(parent)) {
			int child_index = matrix.rawIndex(child);

			int cost = lookup[matrix[parent] - 'A'][child.value() - 'A'];
			flowgraph.connect_both(index, child_index, {-cost, 1, 0}, {cost, 0, 0});
		}
	}

	int result = 0;
	int count = 0;

	std::vector<int> prev(size), dist(size);
	std::vector<decltype(flowgraph)::child> edges(size);
	std::vector<bool> in_queue(size, false);
	std::queue<int> queue;

	while (true) {
		std::fill(prev.begin(), prev.end(), -1);
		std::fill(dist.begin(), dist.end(), std::numeric_limits<int>::max());

		dist[source] = 0;
		in_queue[source] = true;
		queue.push(source);

		while (!queue.empty()) {
			int parent = queue.front();
			queue.pop();
			in_queue[parent] = false;
			for (const auto& child: flowgraph.children(parent)) {
				if (child.edge().capacity > 0
						&& dist[child] > dist[parent] + child.edge().cost) {
					dist[child] = dist[parent] + child.edge().cost;
					edges[child] = child;
					prev[child] = parent;

					if (!in_queue[child]) {
						queue.push(child);
						in_queue[child] = true;
					}
				}
			}
		}

		if (prev[sink] == -1) break;

		int max_flow = std::numeric_limits<int>::max();

		for (int i = sink; i != source; i = prev[i])
			max_flow = std::min(max_flow, edges[i].edge().capacity);

		for (int i = sink; i != source; i = prev[i]) {
			result += max_flow * edges[i].edge().cost;
			edges[i].edge().capacity -= max_flow;
			flowgraph.reverse(edges[i]).edge().capacity += max_flow;
		}
		count++;
	}

	std::cout << - result;
}
