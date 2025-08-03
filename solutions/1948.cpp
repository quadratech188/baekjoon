#include "../modules/FastIO2.h"
#include "../modules/ListGraph2.h"
#include <limits>
#include <queue>
#include <variant>

int main() {
	uint n, m;
	Fast::cin >> n >> m;

	ListGraph<std::monostate, int> graph(n);

	std::vector<int> dependencies(n);

	for (uint i = 0; i < m; i++) {
		uint a, b;
		int t;
		Fast::cin >> a >> b >> t;

		graph.connect(a - 1, b - 1, t);
		dependencies[b - 1] ++;
	}

	uint start, end;
	Fast::cin >> start >> end;
	start --;
	end --;

	std::queue<uint> queue;

	std::vector<int> distances(n);

	std::vector<std::vector<uint>> parents(n);

	queue.push(start);
	while (!queue.empty()) {
		uint parent = queue.front();
		queue.pop();

		for (auto& child: graph.children(parent)) {
			if (distances[parent] + child.edge() > distances[child]) {
				parents[child].clear();
				parents[child].push_back(parent);

				distances[child] = distances[parent] + child.edge();
			}
			else if (distances[parent] + child.edge() == distances[child])
				parents[child].push_back(parent);

			dependencies[child] --;

			if (dependencies[child] == 0)
				queue.push(child);
		}
	}

	std::vector<bool> visited(n, false);
	int cnt = 0;

	queue.push(end);
	while (!queue.empty()) {
		uint child = queue.front();
		queue.pop();

		for (uint parent: parents[child]) {
			cnt ++;

			if (visited[parent]) continue;
			queue.push(parent);
			visited[parent] = true;
		}
	}

	std::cout << distances[end] << '\n' << cnt;
}
