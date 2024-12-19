#include <iostream>
#include <queue>
#include <string>

#include "modules/MatrixGraph.h"
#include "modules/GraphAlgs.h"
#include "modules/Types.h"

int main() {
	char l;
	int n;

	std::cin >> l >> n;

	int k = l - 'a' + 1;

	MatrixGraph<None, int> graph(k);

	std::string prevStr, str;

	for (int i = 0; i < n; i++) {
		std::cin >> str;

		int ptr = 0;

		while (true) {
			if (ptr >= prevStr.size() || ptr >= str.size()) break;

			if (prevStr[ptr] != str[ptr]) {
				graph.edge(prevStr[ptr] - 'a', str[ptr] - 'a') = 1;
				break;
			}
			ptr ++;
		}
		prevStr = str;
	}

	std::vector<int> dependencies = GraphAlgs::dependencies(graph);
	std::queue<int> queue;
	std::vector<char> result;
	result.reserve(k);

	for (int i = 0; i < k; i++) {
		if (dependencies[i] == 0)
			queue.push(i);
	}

	while (!queue.empty()) {
		if (queue.size() > 1) {
			std::cout << "AMBIGUOUS";
			return 0;
		}

		int parent = queue.front();
		queue.pop();

		result.push_back('a' + parent);

		graph.forEachChild(parent, [&queue, &dependencies](int child, None& value) {
				dependencies[child] --;
				if (dependencies[child] == 0)
					queue.push(child);
				});
	}

	if (result.size() != k)
		std::cout << "IMPOSSIBLE";
	else {
		for (char ch: result)
			std::cout << ch;
	}
}
