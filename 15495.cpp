#include <iostream>
#include <queue>
#include "modules/Input.h"
#include "modules/Output.h"
#include "modules/ListGraph.h"
#include "modules/GraphAlgs.h"
#include "modules/Types.h"

struct Recipe {
	std::vector<int> reactants;
	std::vector<int> products;
};

int main() {
	int n, m, k;
	std::cin >> n >> m;

	std::vector<int> initial;
	initial.reserve(m);

	for (int i = 0; i < m; i++) {
		int temp;
		std::cin >> temp;
		initial.push_back(temp);
	}

	std::cin >> k;

	ListGraph<Recipe> graph(k);

	std::vector<int> dependencies;

	for (int i = 0; i < k; i++) {
		int l, r;
		std::cin >> l >> r;
		graph[i].products = Input::inlineToVec<int>(l);
		graph[i].reactants = Input::inlineToVec<int>(r);

		dependencies[i] = l;
	}

	std::vector<int> result;

	std::queue<int> queue;

	for (int i: initial) {
		dependencies[i] = -1; // Prevent them from being made again
		queue.push(i);
	}

	while (!queue.empty()) {
		int parent = queue.front();
		queue.pop();

		result.push_back(parent);

		graph.forEachChild(parent, [&dependencies, &queue, parent](int child, None& value) {
				dependencies[child] --;
				if (dependencies[child] == 0)
					queue.push(child);
				});
	}

	std::cout << result.size() << '\n';
	std::cout << result;
}
