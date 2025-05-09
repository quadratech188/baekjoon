#include "../modules/GridGraph.h"
#include "../modules/ListGraph2.h"
#include "../modules/FastIO.h"
#include <iostream>
#include <limits>
#include <queue>
#include <variant>

struct Edge {
	long long int capacity;
};

int main() {
	FastIO();
	size_t n, m;
	std::cin >> n >> m;

	Matrix<char> map(m, n);
	std::cin >> map;

	GridGraph grid(map);

	std::array<long long int, 26> costs;
	for (int i = 0; i < 26; i++) {
		std::cin >> costs[i];
		costs[i] += 1'000'000'001;
	}

	size_t const size = 2 * m * n + 1;

	size_t const source = 2 * m * n;
	size_t sink;

	ListGraph<int, Edge>
		::reversible<true> flowgraph(size);

	for (Int2 parent: map.bounds()) {
		if (map[parent] == '-') continue;

		size_t const in = 2 * map.rawIndex(parent);
		size_t const out = 2 * map.rawIndex(parent) + 1;

		if (map[parent] == '*')
			sink = in;
		else
		 	flowgraph.connect_both(in, out,
					{costs[map[parent] - 'A']},
					{0});

		for (Int2 child: grid.children(parent)) {
			if (map[child] == '-') continue;

			flowgraph.connect_both(out, 2 * map.rawIndex(child),
					{std::numeric_limits<long long int>::max()},
					{0});
		}

		if (parent.x == 0 || parent.x == m - 1 || parent.y == 0 || parent.y == n - 1)
			flowgraph.connect_both(source, in,
					{std::numeric_limits<long long int>::max()},
					{0});
	}

	long long int cuts = 0;

	std::vector<int> prev(size);
	std::vector<decltype(flowgraph)::child*> edges(size);
	std::queue<int> queue;

	while (true) {
		std::fill(prev.begin(), prev.end(), -1);

		queue.push(source);

		while (!queue.empty()) {
			int parent = queue.front();
			queue.pop();
			for (auto& child: flowgraph.children(parent)) {
				if (child.edge().capacity > 0 && prev[child] == -1) {
					edges[child] = &child;
					prev[child] = parent;

					queue.push(child);
				}
			}
		}

		if (prev[sink] == -1) break;

		long long int max_flow = std::numeric_limits<long long int>::max();

		for (size_t i = sink; i != source; i = prev[i])
			max_flow = std::min(max_flow, edges[i]->edge().capacity);

		for (size_t i = sink; i != source; i = prev[i]) {
			edges[i]->edge().capacity -= max_flow;;
			flowgraph.reverse(*edges[i]).edge().capacity += max_flow;
		}
		cuts += max_flow % 1'000'000'001;
	}

	std::cout << cuts % 1'000'000'001;
}
