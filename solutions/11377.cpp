#include "modules/Matrix.h"
#include "modules/ListGraph.h"
#include "modules/Types.h"
#include "modules/FastIO.h"
#include <iostream>
#include <limits>
#include <queue>

int main() {
	FastIO();
	int n, m, k;
	std::cin >> n >> m >> k;


	int const source = n + m;
	int const sink = n + m + 1;
	int const k_node = n + m + 2;
	int const size = n + m + 3;

	Matrix<int> capacity(size, size);
	Matrix<int> cost(size, size);
	Matrix<int> flow(size, size);

	ListGraph<None, None> graph(n + m + 3);

	graph.connect(source, k_node);
	graph.connect(k_node, source);
	capacity(source, k_node) = k;

	for (int i = 0; i < n; i++) {
		graph.connect(source, i);
		graph.connect(i, source);
		capacity(source, i) = 1;

		graph.connect(k_node, i);
		graph.connect(i, k_node);
		capacity(k_node, i) = 1;
	}

	for (int i = n; i < n + m; i++) {
		graph.connect(i, sink);
		graph.connect(sink, i);
		capacity(i, sink) = 1;
	}

	for (int worker = 0; worker < n; worker++) {
		int size;
		std::cin >> size;
		for (int j = 0; j < size; j++) {
			int job;
			std::cin >> job;
			job --;
			job += n;
			cost(worker, job) = 1;
			cost(job, worker) = -1;
			capacity(worker, job) = 1;
			graph.connect(worker, job);
			graph.connect(job, worker);
		}
	}
	int count = 0;
	std::vector<int> prev(size);
	std::queue<int> queue;

	while (true) {
		std::fill(prev.begin(), prev.end(), -1);
		queue.push(source);

		while (!queue.empty()) {
			int parent = queue.front();
			queue.pop();
			for (auto child: graph.children(parent)) {
				if (prev[child] == -1 && capacity(parent, child) > flow(parent, child)) {
					prev[child] = parent;
					queue.push(child);
					if (child == sink) break;
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
		count++;
	}
	std::cout << count << '\n';
}
