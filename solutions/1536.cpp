#include "../modules/Graph.h"
#include "../modules/ListGraph2.h"
#include "../modules/FastIO.h"
#include <limits>
#include <queue>

int main() {
	FastIO();
	size_t n, k;
	std::cin >> n >> k;

	size_t const size = 4 * n + 2;
	size_t const source = 4 * n;
	size_t const sink = 4 * n + 1;

	ListGraph<std::monostate, int>
		::reversible<true> flowgraph(size);

	for (size_t boy = 0; boy < n; boy++) {
		flowgraph.connect_both(source, boy, 0, 0);
		flowgraph.connect_both(boy, n + boy, k, 0);
	}

	for (size_t girl = 0; girl < n; girl++) {
		flowgraph.connect_both(2 * n + girl, sink, 0, 0);
		flowgraph.connect_both(3 * n + girl, 2 * n + girl, k, 0);
	}

	for (size_t boy = 0; boy < n; boy++) {
		for (size_t girl = 0; girl < n; girl++) {
			char type;
			std::cin >> type;
			switch(type) {
				case '1':
					flowgraph.connect_both(boy, 2 * n + girl, 1, 0);
					break;
				case '0':
					flowgraph.connect_both(n + boy, 3 * n + girl, 1, 0);
			}
		}
	}

	std::vector<size_t> prev(size);
	std::vector<decltype(flowgraph)::child*> edges(size);
	std::queue<size_t> queue;

	for (size_t r = 1; r <= n + 1; r++){
		for (auto& child: flowgraph.children(source)) {
			child.edge() ++;
		}
		for (size_t girl = 0; girl < n; girl++) {
			for (auto& child: flowgraph.children(2 * n + girl)) {
				if (child == sink)
					child.edge() ++;
			}
		}

		size_t cuts = 0;

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

			for (size_t i = sink; i != source; i = prev[i]) {
				edges[i]->edge() -= max_flow;
				flowgraph.reverse(*edges[i]).edge() += max_flow;
			}
			cuts += max_flow;
		}

		if (cuts != n) {
			std::cout << r - 1;
			break;
		}
	}
}
