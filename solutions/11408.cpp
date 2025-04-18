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

	Matrix<int> capacity(n + m + 2, n + m + 2, 0);
	Matrix<int> cost(n + m + 2, n + m + 2, 0);
	Matrix<int> flow(n + m + 2, n + m + 2, 0);

	ListGraph<None, None> graph(n + m + 2);

	int const source  = n + m;
	int const sink  = n + m + 1;
	int const size = n + m + 2;


	for (int i = 0; i < n; i++) {
		graph.connect(source, i);
		graph.connect(i, source);
		capacity(source, i) = 1;
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
			int job, pay;
			std::cin >> job >> pay;
			job --;
			job += n;
			cost(worker, job) = pay;
			cost(job, worker) = - pay;
			capacity(worker, job) = 1;
			graph.connect(worker, job);
			graph.connect(job, worker);
		}
	}

	int result = 0;
	int count = 0;

	while (true) {
		std::vector<int> prev(size, -1), dist(size, std::numeric_limits<int>::max());
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
				if (capacity(parent, child) > flow(parent, child)
						&& dist[child] > dist[parent] + cost(parent, child)) {
					dist[child] = dist[parent] + cost(parent, child);
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
			max_flow = std::min(max_flow, capacity(prev[i], i) - flow(prev[i], i));

		for (int i = sink; i != source; i = prev[i]) {
			result += max_flow * cost(prev[i], i);
			flow(prev[i], i) += max_flow;
			flow(i, prev[i]) -= max_flow;
		}
		count++;
	}

	std::cout << count << '\n' << result;
}
