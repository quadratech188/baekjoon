#include "../modules/ListGraph2.h"
#include "../modules/FastIO2.h"
#include <iostream>
#include <istream>
#include <limits>
#include <queue>

void loop() {
	size_t n, m;
	Fast::cin >> n >> m;

	size_t const source = 2 * n;
	size_t const sink = 2 * n + 1;
	size_t const size = 2 * n + 2;

	ListGraph<std::monostate, int>
		::reversible<true> graph(size);

	for (size_t i = 0; i < n; i++) {
		graph.connect_both(source, i, 1, 0);
		graph.connect_both(i + n, sink, 1, 0);
	}

	for (size_t i = 0; i < m; i++) {
		size_t u, v;
		Fast::cin >> u >> v;
		graph.connect_both(u, v + n, 1, 0);
	}

	std::vector<int> prev(size);
	std::vector<decltype(graph)::child*> edges(size);
	std::queue<int> queue;

	int count = 0;

	while (true) {
		std::fill(prev.begin(), prev.end(), -1);

		queue.push(source);

		while (!queue.empty()) {
			int parent = queue.front();
			queue.pop();
			for (auto& child: graph.children(parent)) {
				if (child.edge() > 0 && prev[child] == -1) {
					edges[child] = &child;
					prev[child] = parent;

					queue.push(child);
				}
			}
		}

		if (prev[sink] == -1) break;

		for (size_t i = sink; i != source; i = prev[i]) {
			edges[i]->edge() --;
			graph.reverse(*edges[i]).edge() ++;
		}
		count ++;
	}
	std::cout << count << '\n';
}

int main() {
	int t;
	Fast::cin >> t;
	for (int i = 0; i < t; i++) loop();
}
