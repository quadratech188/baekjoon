#include "modules/FastIO.h"
#include "modules/ListGraph.h"
#include "modules/Types.h"
#include "modules/Matrix.h"
#include <limits>
#include <queue>

struct Edge {
	Edge(): capacity(0), cost(0) {}
	Edge(int capacity, int cost):
		capacity(capacity), cost(cost) {}

	int capacity;
	int cost;
};

int main() {
	FastIO();
	int n;
	std::cin >> n;

	const int size = 2 * n + 2;
	const int source = 2 * n;
	const int sink = 2 * n + 1;

	ListGraph<None, Edge> graph(size);
	Matrix<int> flow(size, size, 0);

	for (int i = 0; i < n; i++) {
		graph.connect(source, i, Edge(1, 0));
		graph.connect(i, source);
	}

	for (int i = n; i < 2 * n; i++) {
		graph.connect(i, sink, Edge(1, 0));
		graph.connect(sink, i);
	}

	for (int i = 0; i < n; i++) {
		for (int j = n; j < 2 * n; j++) {
			int cost;
			std::cin >> cost;
			graph.connect(i, j, Edge(1, cost));
			graph.connect(j, i, Edge(0, -cost));
		}
	}

	int result = 0;
	
	while (true) {
		std::vector<int> prev(size, -1), dist(size, std::numeric_limits<int>::max());
		std::vector<Edge*> edges(size);
		std::vector<bool> in_queue(size, false);
		std::queue<int> queue;
		dist[source] = 0;
		in_queue[source] = true;
		queue.push(source);

		while (!queue.empty()) {
			int parent = queue.front();
			queue.pop();
			in_queue[parent] = false;
			for (auto child: graph.children(parent)) {
				if (child.edge().capacity > flow(parent, child)
						&& dist[child] > dist[parent] + child.edge().cost) {
					dist[child] = dist[parent] + child.edge().cost;
					prev[child] = parent;
					edges[child] = &child.edge();

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
			max_flow = std::min(max_flow, edges[i]->capacity - flow(prev[i], i));

		for (int i = sink; i != source; i = prev[i]) {
			result += max_flow * edges[i]->cost;
			flow(prev[i], i) += max_flow;
			flow(i, prev[i]) -= max_flow;
		}
	}

	std::cout << result;
}
