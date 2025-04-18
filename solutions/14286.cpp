#include "modules/Matrix.h"
#include "modules/ListGraph.h"
#include "modules/Types.h"
#include "modules/FastIO.h"
#include <iostream>
#include <limits>
#include <queue>

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;

	Matrix<int> capacity(n, n, 0);
	Matrix<int> cost(n, n, 1);
	Matrix<int> flow(n, n, 0);

	ListGraph<None, None> graph(n);

	for (int i = 0; i < m; i++) {
		int a, b, c;
		std::cin >> a >> b >> c;
		graph.connect(a - 1, b - 1);
		graph.connect(b - 1, a - 1);
		capacity(a - 1, b - 1) = c;
		capacity(a - 1, b - 1) = c;
	}

	int source, sink;
	std::cin >> source >> sink;
	source--;
	sink--;

	int count = 0;

	while (true) {
		std::vector<int> prev(graph.size(), -1);
		std::queue<int> queue;
		queue.push(source);

		while (!queue.empty()) {
			int parent = queue.front();
			queue.pop();
			for (auto child: graph.children(parent)) {
				if (capacity(parent, child) > flow(parent, child)
						&& prev[child] == -1) {
					prev[child] = parent;
					queue.push(child);
				}
			}
		}

		if (prev[sink] == -1) break;

		int max_flow = std::numeric_limits<int>::max();

		for (int i = sink; i != source; i = prev[i])
			max_flow = std::min(max_flow, capacity(prev[i], i) - flow(prev[i], i));

		for (int i = sink; i != source; i = prev[i]) {
			flow(prev[i], i) += max_flow;
			flow(i, prev[i]) -= max_flow;
		}
		count += max_flow;
	}

	std::cout << count;
}
