#include "modules/MatrixGraph.h"
#include "modules/GraphAlgs.h"
#include "modules/Types.h"
#include "modules/Input.h"
#include "modules/FastIO.h"

#include <queue>

void loop() {
	int n, m;
	std::cin >> n >> m;

	MatrixGraph<int, int> graph(n);

	for (int i = 0; i < m; i++) {
		std::vector<int> rank = Input::inlineToVec<int>(n);

		for (int a = 0; a < n; a++) {
			for (int b = a + 1; b < n; b++) {
				graph.edge(rank[a] - 1, rank[b] - 1) ++;
			}
		}
	}

	for (int a = 0; a < n; a++) {
		for (int b = 0; b < n; b++) {
			graph.edge(a, b) = graph.edge(a, b) == m? 1: 0;
		}
	}

	std::vector<int> dependencies = GraphAlgs::dependencies(graph);
	std::queue<int> queue;

	for (int i = 0; i < n; i++) {
		if (dependencies[i] == 0) {
			queue.push(i);
			graph[i] = 1;
		}
	}

	if (queue.empty()) {
		std::cout << 1 << '\n';
		return;
	}

	while (!queue.empty()) {
		int parent = queue.front();
		queue.pop();

		graph.forEachChild(parent, [&queue, &dependencies, &graph, parent](int child, int& value) {
				value = std::max(value, graph[parent] + 1);
				dependencies[child] --;
				if (dependencies[child] == 0)
					queue.push(child);
				});
	}

	int max = 0;
	for (int i = 0; i < n; i++) {
		max = std::max(max, graph[i]);
	}

	std::cout << max << '\n';
}

int main() {
	FastIO();
	int c;
	std::cin >> c;
	for (int i = 0; i < c; i++)
		loop();
}
