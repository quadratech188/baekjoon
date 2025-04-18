#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
#include "modules/MatrixGraph.h"
#include "modules/GraphAlgs.h"
#include "modules/Types.h"

int main() {
	int n, k;
	std::cin >> n >> k;

	std::vector<int> times(k, 0);

	MatrixGraph<None, int> graph(k);

	for (int i = 0; i < n; i++) {
		std::string str;
		std::cin >> str;

		for (int a = 0; a < str.size(); a++) {
			for (int b = a + 1; b < str.size(); b++) {
				graph.edge(str[a] - 'A', str[b] - 'A') ++;
			}
		}
	}

	for (int parent = 0; parent < k; parent++) {
		for (int child = 0; child < k; child++) {
			graph.edge(parent, child) = graph.edge(parent, child) == n? 1: 0;
		}
	}

	std::vector<int> dependencies = GraphAlgs::dependencies(graph);

	std::queue<int> queue;
	std::vector<int> depth(k, 0);

	for (int i = 0; i < k; i++) {
		if (dependencies[i] == 0) {
			queue.push(i);
			depth[i] = 1;
		}
	}

	if (queue.empty()) {
		std::cout << 1;
		return 0;
	}

	while (!queue.empty()) {
		int parent = queue.front();
		queue.pop();

		graph.forEachChild(parent, [&queue, &depth, &dependencies, &parent](int child, None& value) {
				depth[child] = std::max(depth[child], depth[parent] + 1);

				dependencies[child] --;
				if (dependencies[child] == 0)
					queue.push(child);
				});
	}

	std::cout << *std::max_element(depth.begin(), depth.end()) << '\n';
}
