#include "modules/ListGraph.h"
#include "modules/Types.h"
#include "modules/Matrix.h"
#include "modules/FastIO.h"
#include <iostream>
#include <limits>
#include <queue>

 // https://m.blog.naver.com/kks227/220810623254
 // wtf

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;

	ListGraph<None, None> graph(n + m + 2);
	Matrix<int> capacity(graph.size(), graph.size(), 0);
	Matrix<int> cost(graph.size(), graph.size(), 0);
	Matrix<int> flow(graph.size(), graph.size(), 0);

	int source = n + m;
	int sink = n + m + 1;

	for (int i = m; i < n + m; i++) {
		std::cin >> capacity(i, sink);
		graph.connect(sink, i);
		graph.connect(i, sink);
	}

	for (int i = 0; i < m; i++) {
		std::cin >> capacity(source, i);
		graph.connect(source, i);
		graph.connect(i, source);
	}

	for (int i = 0; i < m; i++) {
		for (int j = m; j < m + n; j++) {
			std::cin >> cost(i, j);
			cost(j, i) = - cost(i, j);
			capacity(i, j) = std::numeric_limits<int>::max();
			graph.connect(i, j);
			graph.connect(j, i);
		}
	}

	int result = 0;

	while (true) {
		std::vector<int> prev(graph.size(), -1);
		std::vector<int> accumulative_cost(graph.size(), std::numeric_limits<int>::max());
		std::vector<bool> in_queue(graph.size(), false);

		std::queue<int> queue;

		accumulative_cost[source] = 0;
		in_queue[source] = true;
		queue.push(source);

		while (!queue.empty()) {
			int parent = queue.front();
			queue.pop();
			in_queue[parent] = false;
			for (auto child: graph.children(parent)) {
				if (capacity(parent, child) > flow(parent, child)
						&& accumulative_cost[child] > accumulative_cost[parent] + cost(parent, child)) {
					accumulative_cost[child] = accumulative_cost[parent] + cost(parent, child);
					prev[child] = parent;

					if (!in_queue[child]) {
						queue.push(child);
						in_queue[child] = true;
					}
				}
			}
		}

		if (prev[sink] == -1) break;

		int min_flow = std::numeric_limits<int>::max();
		for (int i = sink; i != source; i = prev[i])
			min_flow = std::min(min_flow, capacity(prev[i], i) - flow(prev[i], i));

		for (int i = sink; i != source; i = prev[i]) {
			result += min_flow * cost(prev[i], i);
			flow(prev[i], i) += min_flow;
			flow(i, prev[i]) -= min_flow;
		}
	}

	std::cout << result;
}
