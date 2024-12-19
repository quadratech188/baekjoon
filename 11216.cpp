#include "modules/MatrixGraph.h"
#include "modules/GraphAlgs.h"
#include "modules/Types.h"
#include "modules/Input.h"
#include "modules/FastIO.h"

#include <queue>

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;

	MatrixGraph<int, int> graph(n);

	for (int i = 0; i < m; i++) {
		int a, b;
		std::cin >> a >> b;
		graph.edge(a - 1, b - 1) = 1;
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
		std::cout << 1;
		return 0;
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

	std::cout << max;
}
