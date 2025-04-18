#include <iostream>
#include <queue>

#include "modules/Input.h"
#include "modules/ListGraph.h"
#include "modules/GraphAlgs.h"
#include "modules/Types.h"
#include "modules/FastIO.h"

// Counterexample: 
// 1
// 2 0
// 1 2
// -> 0 (expected 1)

void loop() {
	int n, m;
	std::cin >> n >> m;

	ListGraph<None> graph(n);

	std::vector<int> lookup = Input::inlineToVec<int>(n);

	for (int i = 0; i < m; i++) {
		int a, b;
		std::cin >> a >> b;

		graph.connect(a - 1, b - 1);
	}

	std::vector<int> dependencies = GraphAlgs::dependencies(graph);
	std::queue<int> queue;
	std::vector<int> depth(n);

	for (int i = 0; i < n; i++) {
		if (dependencies[i] == 0) {
			queue.push(i);
		}
	}

	while (!queue.empty()) {
		int parent = queue.front();
		queue.pop();

		graph.forEachChild(parent, [&depth, &dependencies, &queue, &lookup, parent](int child, None& value){
				if (lookup[parent] == lookup[child])
					depth[child] = std::max(depth[child], depth[parent]);
				else
					depth[child] = std::max(depth[child], depth[parent] + 1);
				dependencies[child] --;
				if (dependencies[child] == 0)
					queue.push(child);
				});
	}

	int endsWith1 = -1;
	int endsWith2 = -1;

	for (int i = 0; i < n; i++) {
		if (lookup[i] == 1)
			endsWith1 = std::max(depth[i], endsWith1);
		else
		 	endsWith2 = std::max(depth[i], endsWith2);
	}

	std::cout << std::min(std::max(endsWith1, endsWith2 + 1), std::max(endsWith2, endsWith1 + 1)) << '\n';
}

int main() {
	FastIO();
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++)
		loop();
}
