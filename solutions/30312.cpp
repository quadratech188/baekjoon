#include <iostream>
#include <queue>
#include <string>

#include "modules/MatrixGraph.h"
#include "modules/GraphAlgs.h"
#include "modules/Types.h"

int main() {
	int n;
	std::cin >> n;

	MatrixGraph<None, int> graph(26);

	std::string prevStr, str;

	for (int i = 0; i < n; i++) {
		std::cin >> str;

		int ptr = 0;

		while (true) {
			if (ptr == str.size() && prevStr.size() > str.size()) {
				std::cout << "impossible";
				return 0;
			}

			if (ptr >= str.size() || ptr >= prevStr.size())
				break;

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
	result.reserve(26);

	for (int i = 0; i < 26; i++) {
		if (dependencies[i] == 0)
			queue.push(i);
	}

	while (!queue.empty()) {
		int parent = queue.front();
		queue.pop();

		result.push_back('a' + parent);

		graph.forEachChild(parent, [&queue, &dependencies](int child, None& value) {
				dependencies[child] --;
				if (dependencies[child] == 0)
					queue.push(child);
				});
	}

	if (result.size() != 26)
		std::cout << "impossible";
	else {
		for (char ch: result)
			std::cout << ch;
	}
}
