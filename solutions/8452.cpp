#include "modules/ListGraph.h"
#include "modules/Input.h"
#include "modules/Types.h"
#include "modules/Vec2.h"
#include "modules/FastIO.h"
#include <limits>
#include <queue>
#include <stack>

void updateDistances(ListGraph<int, None>& graph, int root) {
	std::queue<int> queue;
	queue.push(root);

	while (!queue.empty()) {
		int parent = queue.front();
		queue.pop();

		for (int child: graph.children(parent)) {
			if (graph[child] > graph[parent] + 1) {
				graph[child] = graph[parent] + 1;
				queue.push(child);
			}
		}
	}
}

int main() {
	FastIO();
	int n, m, q;
	std::cin >> n >> m >> q;

	ListGraph<int, None> graph(n, std::numeric_limits<int>::max() - 10);
	graph[0] = 0;

	std::vector<Int2> edges = Input::inlineToVec<Int2>(m);

	std::vector<std::pair<char, int>> queries = Input::inlineToVec<std::pair<char, int>>(q);

	std::vector<bool> deleted(m);

	for (auto query: queries) {
		if (query.first == 'U')
			deleted[query.second - 1] = true;
	}

	for (int i = 0; i < m; i++) {
		if (!deleted[i])
			graph.connect(edges[i].x - 1, edges[i].y - 1);
	}

	updateDistances(graph, 0);

	std::stack<int> results;

	for (int i = q - 1; i >= 0; i--) {
		char type = queries[i].first;
		int data = queries[i].second;

		if (type == 'U') {
			graph.connect(edges[data - 1].x - 1, edges[data - 1].y - 1);
			updateDistances(graph, edges[data - 1].x - 1);
		}
		else {
			if (graph[data - 1] == std::numeric_limits<int>::max() - 10)
				results.push(-1);
			else
				results.push(graph[data - 1]);
		}
	}

	while (!results.empty()) {
		std::cout << results.top() << '\n';
		results.pop();
	}
}
