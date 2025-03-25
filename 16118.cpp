#include "modules/ListGraph.h"
#include "modules/FastIO.h"
#include <iostream>
#include <limits>
#include <queue>
#include <variant>
#include <vector>
int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;

	ListGraph<std::monostate, int> graph(n);

	for (int i = 0; i < m; i++) {
		int a, b, d;
		std::cin >> a >> b >> d;
		graph.connect(a - 1, b - 1, d);
		graph.connect(b - 1, a - 1, d);
	}

	std::priority_queue<std::tuple<int, int, int>> queue;
	std::vector<int> fox_times(n, std::numeric_limits<int>::max());
	fox_times[0] = 0;
	queue.emplace(0, 0, 0);

	while (!queue.empty()) {
		auto [time, parent, turn] = queue.top();
		time = - time;
		queue.pop();

		if (fox_times[parent] < time) continue;

		for (auto child: graph.children(parent)) {
			int next_time = time + child.edge() * 2;
			if (fox_times[child] > next_time) {
				fox_times[child] = next_time;
				queue.emplace(- next_time, child, turn + 1);
			}
		}
	}

	std::array<std::vector<int>, 2> wolf_times;
	wolf_times[0].assign(n, std::numeric_limits<int>::max());
	wolf_times[1].assign(n, std::numeric_limits<int>::max());
	wolf_times[0][0] = 0;
	queue.emplace(0, 0, 0);

	while (!queue.empty()) {
		auto [time, parent, turn] = queue.top();
		time = - time;
		queue.pop();

		if (wolf_times[turn % 2][parent] < time) continue;

		if (turn % 2 == 0) {
			for (auto child: graph.children(parent)) {
				int next_time = time + child.edge();
				if (wolf_times[1][child] > next_time) {
					wolf_times[1][child] = next_time;
					queue.emplace(- next_time, child, turn + 1);
				}
			}
		}
		else {
			for (auto child: graph.children(parent)) {
				int next_time = time + 4 * child.edge();
				if (wolf_times[0][child] > next_time) {
					wolf_times[0][child] = next_time;
					queue.emplace(- next_time, child, turn + 1);
				}
			}
		}
	}
	
	int count = 0;
	for (int i = 1; i < n; i++) {
		if (fox_times[i] < std::min(wolf_times[0][i], wolf_times[1][i])) count++;
	}

	std::cout << count;
}
