#include "../modules/ListGraph2.h"
#include "../modules/FastIO2.h"
#include <iostream>
#include <istream>
#include <limits>
#include <queue>

struct Shark {
	int size, speed, intelligence;

	bool operator>(Shark const& other) const {
		return size >= other.size && speed >= other.speed && intelligence >= other.intelligence;
	}
	bool operator==(Shark const& other) const {
		return size == other.size && speed == other.speed && intelligence == other.intelligence;
	}
};

int main() {
	size_t n;
	Fast::cin >> n;

	std::vector<Shark> sharks(n);
	for (auto& shark: sharks)
		Fast::cin >> shark.size >> shark.speed >> shark.intelligence;

	size_t const source = 2 * n;
	size_t const sink = 2 * n + 1;
	size_t const size = 2 * n + 2;

	ListGraph<std::monostate, int>
		::reversible<true> graph(size);

	for (size_t parent = 0; parent < n; parent++) {
		for (size_t child = 0; child < n; child++) {
			if (sharks[parent] == sharks[child]) {
				if (parent < child)
					graph.connect_both(parent, child + n, 1, 0);
			}
			else if (sharks[parent] > sharks[child])
				graph.connect_both(parent, child + n, 1, 0);
		}
		graph.connect_both(source, parent, 2, 0);
		graph.connect_both(parent + n, sink, 1, 0);
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
	std::cout << n - count;
}
