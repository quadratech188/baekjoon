#include "../modules/Graph.h"
#include "../modules/ListGraph2.h"
#include "../modules/FastIO.h"
#include <queue>
#include <numeric>

struct Edge {
	int cap = 0;
	int flow = 0;

	int residual() const {
		return cap - flow;
	}
};

template <Graph G>
bool check(size_t r, size_t n, G& graph) {
	size_t const size = 6 * n + 2;
	size_t const source = 6 * n;
	size_t const sink = 6 * n + 1;

	for (size_t parent = 0; parent < graph.size(); ++parent) {
		for (auto& child : graph.children(parent)) {
			child.edge().flow = 0;
		}
	}

	for (auto& child : graph.children(source)) {
		child.edge().cap = r;
	}
	for (size_t i = 0; i < n; ++i) {
		for (auto& child : graph.children(3 * n + i)) {
			child.edge().cap = r;
		}
	}

	std::vector<size_t> prev(size);
	std::vector<typename G::child*> edges(size);
	std::queue<size_t> queue;

	size_t total_flow = 0;
	while (true) {
		std::fill(prev.begin(), prev.end(), std::numeric_limits<size_t>::max());

		queue.push(source);
		while (!queue.empty()) {
			size_t cur = queue.front(); queue.pop();
			for (auto& child : graph.children(cur)) {
				if (child.edge().residual() > 0 && prev[child] == std::numeric_limits<size_t>::max()) {
					prev[child] = cur;
					edges[child] = &child;
					queue.push(child);
				}
			}
		}

		if (prev[sink] == std::numeric_limits<size_t>::max()) break;

		int flow = std::numeric_limits<int>::max();
		for (size_t i = sink; i != source; i = prev[i]) {
			flow = std::min(flow, edges[i]->edge().residual());
		}
		for (size_t i = sink; i != source; i = prev[i]) {
			edges[i]->edge().flow += flow;
			graph.reverse(*edges[i]).edge().flow -= flow;
		}
		total_flow += flow;
	}

	return total_flow == r * n;
}

int main() {
	FastIO();
	size_t n;
	int k;
	std::cin >> n >> k;

	size_t const size = 6 * n + 2;
	size_t const source = 6 * n;
	size_t const sink = 6 * n + 1;
	int const INF = std::numeric_limits<int>::max();

	ListGraph<std::monostate, Edge>::reversible<true> graph(size);

	for (size_t i = 0; i < n; i++) {
		graph.connect_both(source, i, {0, 0}, {0, 0});
		graph.connect_both(i, n + i, {INF, 0}, {0, 0});
		graph.connect_both(i, 2 * n + i, {k, 0}, {0, 0});
		graph.connect_both(3 * n + i, sink, {0, 0}, {0, 0});
		graph.connect_both(4 * n + i, 3 * n + i, {INF, 0}, {0, 0});
		graph.connect_both(5 * n + i, 3 * n + i, {k, 0}, {0, 0});
	}

	for (size_t boy = 0; boy < n; boy++) {
		for (size_t girl = 0; girl < n; girl++) {
			char type;
			std::cin >> type;
			if (type == '1') {
				graph.connect_both(n + boy, 4 * n + girl, {1, 0}, {0, 0});
			} else {
				graph.connect_both(2 * n + boy, 5 * n + girl, {1, 0}, {0, 0});
			}
		}
	}

	size_t l = 0, r = n, ans = 0;
	while (l <= r) {
		size_t mid = (l + r) / 2;
		if (check(mid, n, graph)) {
			ans = mid;
			l = mid + 1;
		} else {
			r = mid - 1;
		}
	}

	std::cout << ans;
}

