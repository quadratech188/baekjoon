#include "../modules/GridGraph.h"
#include "../modules/ListGraph2.h"
#include "../modules/FastIO.h"
#include <iostream>
#include <limits>
#include <queue>

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;
	Matrix<char> map(m, n);
	std::cin >> map;

	GridGraph graph(map);

	size_t const size = 2 * m * n;
	size_t source, sink;

	ListGraph<std::monostate, int>
		::reversible<true> flowgraph(size);

	for (Int2 parent: map.bounds()) {
		if (map[parent] == '#') continue;
		size_t in = 2 * map.rawIndex(parent);
		size_t out = 2 * map.rawIndex(parent) + 1;
		flowgraph.connect_both(in, out, 1, 0);

		for (Int2 child: graph.children(parent)) {
			if (map[child] == '#') continue;
			flowgraph.connect_both(out, 2 * map.rawIndex(child), std::numeric_limits<int>::max(), 0);
		}

		if (map[parent] == 'K')
			source = out;
		else if (map[parent] == 'H')
			sink = in;
	}

	std::vector<size_t> prev(size);
	std::vector<decltype(flowgraph)::child*> edges(size);
	std::queue<size_t> queue;

	int cuts = 0;

	while (true) {
		std::fill(prev.begin(), prev.end(), std::numeric_limits<size_t>::max());

		queue.push(source);

		while (!queue.empty()) {
			int parent = queue.front();
			queue.pop();
			for (auto& child: flowgraph.children(parent)) {
				if (child.edge() > 0 && prev[child] == std::numeric_limits<size_t>::max()) {
					edges[child] = &child;
					prev[child] = parent;

					queue.push(child);
				}
			}
		}

		if (prev[sink] == std::numeric_limits<size_t>::max()) break;

		int max_flow = std::numeric_limits<int>::max();

		for (size_t i = sink; i != source; i = prev[i])
			max_flow = std::min(max_flow, edges[i]->edge());

		if (max_flow == std::numeric_limits<int>::max()) {
			cuts = -1;
			break;
		}

		for (size_t i = sink; i != source; i = prev[i]) {
			edges[i]->edge() -= max_flow;;
			flowgraph.reverse(*edges[i]).edge() += max_flow;
		}
		cuts += max_flow;
	}

	std::cout << cuts;
}
