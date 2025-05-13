#include "../modules/FastIO2.h"
#include "../modules/ListGraph2.h"
#include <iostream>
#include <limits>
#include <queue>
#include <variant>

int main() {
	size_t n, p;
	Fast::cin >> n >> p;

	ListGraph<std::monostate, int>
		::reversible<true> graph(2 * n);

	for (size_t i = 0; i < n; i++) {
		graph.connect_both(2 * i, 2 * i + 1, 1, 0);
	}

	for (size_t i = 0; i < p; i++) {
		size_t a, b;
		Fast::cin >> a >> b;
		graph.connect_both(2 * a - 1, 2 * b - 2, std::numeric_limits<int>::max(), 0);
		graph.connect_both(2 * b - 1, 2 * a - 2, std::numeric_limits<int>::max(), 0);
	}

	size_t const source = 2 * 0 + 1;
	size_t const sink = 2 * 1;
	size_t const size = 2 * n;

	std::vector<size_t> prev(size);
	std::vector<decltype(graph)::child*> edges(size);
	std::queue<size_t> queue;

	int count = 0;

	while (true) {
		std::fill(prev.begin(), prev.end(), std::numeric_limits<size_t>::max());

		queue.push(source);

		while (!queue.empty()) {
			size_t parent = queue.front();
			queue.pop();

			for (auto& child: graph.children(parent)) {
				if (child.edge() > 0 && prev[child] == std::numeric_limits<size_t>::max()) {
					prev[child] = parent;
					edges[child] = &child;
					queue.push(child);
				}
			}
		}
		if (prev[sink] == std::numeric_limits<size_t>::max()) break;

		for (size_t i = sink; i != source; i = prev[i]) {
			edges[i]->edge() --;
			graph.reverse(*edges[i]).edge() ++;
		}
		count++;
	}
	std::cout << count;
}
