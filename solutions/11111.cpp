#include "../modules/GridGraph.h"
#include "../modules/ListGraph2.h"
#include "../modules/FastIO.h"
#include <iostream>
#include <limits>
#include <queue>
#include <set>

constexpr int lookup[6][6] = {
	10, 8, 7, 5, 0, 1,
	8, 6, 4, 3, 0, 1,
	7, 4, 3, 2, 0, 1,
	5, 3, 2, 2, 0, 1,
	0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 0, 0
};

struct Edge {
	int cost;
	int capacity;
};

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;

	Matrix<char> matrix(m, n);
	std::cin >> matrix;
	GridGraph<decltype(matrix)> graph(matrix);

	int const source = n * m;
	int const sink = n * m + 1;
	int const size = n * m + 2;
	ListGraph<std::monostate, Edge>
		::reversible<true> flowgraph(size);

	for (Int2 parent: matrix.bounds()) {
		int index = matrix.rawIndex(parent);

		flowgraph.connect_both(index, sink, {0, 1}, {0, 0});

		if ((parent.x + parent.y) % 2 != 0) continue;

		flowgraph.connect_both(source, index, {0, 1}, {0, 0});

		for (auto child: graph.children(parent)) {
			int child_index = matrix.rawIndex(child);

			int cost = lookup[matrix[parent] - 'A'][child.value() - 'A'];
			flowgraph.connect_both(index, child_index, {-cost, 1}, {cost, 0});
		}
	}

	int result = 0;
	int count = 0;

	std::vector<int> prev(size), dist(size);
	std::vector<decltype(flowgraph)::child*> edges(size);
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
			for (auto& child: flowgraph.children(parent)) {
				if (child.edge().capacity > 0
						&& dist[child] > dist[parent] + child.edge().cost) {
					dist[child] = dist[parent] + child.edge().cost;
					edges[child] = &child;
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
			max_flow = std::min(max_flow, edges[i]->edge().capacity);

		for (int i = sink; i != source; i = prev[i]) {
			result += max_flow * edges[i]->edge().cost;
			edges[i]->edge().capacity -= max_flow;
			flowgraph.reverse(*edges[i]).edge().capacity += max_flow;
		}
		count++;
	}

	std::cout << - result;
}
