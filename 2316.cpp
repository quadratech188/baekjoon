#include "modules/Matrix.h"
#include "modules/ListGraph.h"
#include "modules/Types.h"
#include "modules/FastIO.h"
#include <iostream>
#include <limits>
#include <queue>

int main() {
	FastIO();
	int n, p;
	std::cin >> n >> p;

	std::vector<int> usable(n, 1);

	ListGraph<None, None> graph(n);

	int const source = 0;
	int const sink = 1;

	for (int i = 0; i < p; i++) {
		int a, b;
		std::cin >> a >> b;
		graph.connect(a - 1, b - 1);
		graph.connect(b - 1, a - 1);
	}

	int count = 0;

	while (true) {
		std::vector<int> prev(n, -1);
		std::vector<bool> in_queue(n, false);
		std::queue<int> queue;
		in_queue[source] = true;
		queue.push(source);

		while (!queue.empty()) {
			int parent = queue.front();
			queue.pop();
			in_queue[parent] = false;
			for (auto child: graph.children(parent)) {
				if (usable[child] && prev[child] == -1) {
					prev[child] = parent;

					if (!in_queue[child]) {
						queue.push(child);
						in_queue[child] = true;
					}
				}
			}
		}

		if (prev[sink] == -1) break;

		for (int i = sink; i != source; i = prev[i]) {
			usable[i] --;
			std::cout << i << '\n';
		}
		count++;
	}

	std::cout << count << '\n';
}
