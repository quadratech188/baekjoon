#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <sys/types.h>
#include <variant>
#include "../modules/FastIO.h"
#include "../modules/FastIO2.h"
#include "../modules/ListGraph2.h"

int main() {
	uint n, m;
	Fast::cin >> n >> m;

	size_t const size = n + m + 2;
	uint const source = n + m;
	uint const sink = n + m + 1;

	ListGraph<std::monostate, int>
		::with_reversible<true>
		::with_index<uint> graph(size);

	int n_sum = 0;
	for (uint i = 0; i < n; i++) {
		int k;
		Fast::cin >> k;
		graph.connect_both(source, i, k, 0);
		n_sum += k;
	}

	int m_sum = 0;
	for (uint j = n; j < n + m; j++) {
		int k;
		Fast::cin >> k;
		graph.connect_both(j, sink, k, 0);
		m_sum += k;
	}

	if (n_sum != m_sum) {
		std::cout << "-1";
		return 0;
	}

	for (uint i = 0; i < n; i++) {
		for (uint j = n; j < n + m; j++)
			graph.connect_both(i, j, 1, 0);
	}

	std::vector<uint> prev(size);
	std::vector<decltype(graph)::child*> edges(size);
	std::queue<uint> queue;

	int flow = 0;
	while (true) {
		std::fill(prev.begin(), prev.end(), std::numeric_limits<uint>::max());

		queue.push(source);

		while (!queue.empty()) {
			uint parent = queue.front();
			queue.pop();

			for (auto& child: graph.children(parent)) {
				if (child.edge() == 0 || prev[child] != std::numeric_limits<uint>::max()) continue;

				prev[child] = parent;
				edges[child] = &child;
				queue.push(child);
			}
		}

		if (prev[sink] == std::numeric_limits<uint>::max()) break;

		for (uint i = sink; i != source; i = prev[i]) {
			edges[i]->edge() --;
			graph.reverse(*edges[i]).edge() ++;
		}
		flow++;
	}

	if (n_sum != flow) {
		std::cout << "-1";
		return 0;
	}

	for (uint a = 0; a < n; a++) {
		for (auto& b: graph.children(a)) {
			if (b == source) continue;

			// unused
			if (b.edge() == 1) continue;

			std::fill(prev.begin(), prev.end(), std::numeric_limits<uint>::max());

			queue.push(a);
			
			while (!queue.empty()) {
				uint parent = queue.front();
				queue.pop();

				if (parent < a) continue;

				for (auto& child: graph.children(parent)) {
					if (child.edge() == 0 || prev[child] != std::numeric_limits<uint>::max()) continue;
					if (parent == a && child < b) continue;

					prev[child] = parent;
					edges[child] = &child;
					queue.push(child);
				}
			}

			if (prev[b] == std::numeric_limits<uint>::max()) continue;

			for (uint i = b; i != a; i = prev[i]) {
				edges[i]->edge() --;
				graph.reverse(*edges[i]).edge() ++;
			}

			b.edge() ++;
			graph.reverse(b).edge() --;
		}
	}

	for (uint i = 0; i < n; i++) {
		for (auto& child: graph.children(i)) {
			if (child == source) continue;
			std::cout << 1 - child.edge();
		}
		std::cout << '\n';
	}
}
