#include <iostream>
#include <queue>
#include <vector>

#include "modules/FastIO.h"
#include "modules/Graph.h"
#include "modules/GraphAlgs.h"
#include "modules/Input.h"
#include "modules/Output.h"
#include "modules/Types.h"

void loop() {
	int n;
	std::cin >> n;
	std::vector<int> teams = Input::inlineToVec<int>(n);
	for (int& team: teams) team --;

	MatrixGraph<None, int> graph(n);

	for (int winner = 0; winner < n; winner++) {
		for (int loser = winner + 1; loser < n; loser++) {
			graph.edge(teams[winner], teams[loser]) = true;
		}
	}

	int m;
	std::cin >> m;
	for (int i = 0; i < m; i++) {
		int a, b;
		std::cin >> a >> b;
		a --;
		b --;

		graph.edge(a, b) = !graph.edge(a, b);
		graph.edge(b, a) = !graph.edge(b, a);
	}

	std::vector<int> dependencies = GraphAlgs::dependencies(graph);

	std::queue<int> queue;

	bool uncertain = false;

	for (int i = 0; i < n; i++) {
		if (dependencies[i] == 0) {
			queue.push(i);
		}
	}

	std::vector<int> result;
	result.reserve(n);

	for (int i = 0; i < n; i++) {
		if (queue.empty()) {
			std::cout << "IMPOSSIBLE\n";
			return;
		}

		if (queue.size() > 1) {
			uncertain = true;
		}

		int parent = queue.front();
		queue.pop();
		result.push_back(parent + 1);

		graph.forEachChild(parent, [&queue, &dependencies, &result](int child, None value) {
				dependencies[child] --;
				if (dependencies[child] == 0) {
					queue.push(child);
				}
				});
	}
	if (uncertain)
		std::cout << "?\n";
	else
		std::cout << result << '\n';
}

int main() {
	FastIO();
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++) {
		loop();
	}
}
