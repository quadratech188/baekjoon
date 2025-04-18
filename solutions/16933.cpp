#include "modules/GridGraph.h"
#include "modules/FastIO.h"
#include <iostream>
#include <limits>
#include <queue>

struct Data {
	Int2 index;
	int breaks_left;
	int time;
};

int main() {
	FastIO();
	int n, m, k;
	std::cin >> n >> m >> k;

	Matrix<char> grid(m, n);
	std::cin >> grid;

	GridGraph<decltype(grid)> graph(grid);

	std::vector<Matrix<int>> times(k + 1, Matrix<int>(m, n, std::numeric_limits<int>::max()));

	std::queue<Data> queue;
	queue.push({
			.index = Int2::zero(),
			.breaks_left = k,
			.time = 0
			});

	while (!queue.empty()) {
		auto [parent, breaks_left, time] = queue.front();
		queue.pop();

		if (parent == Int2(m - 1, n - 1)) {
			std::cout << time + 1;
			return 0;
		}

		if (time % 2 == 1 && breaks_left > 0)
			queue.push({
					.index = parent,
					.breaks_left = breaks_left,
					.time = time + 1
					});

		for (auto child: graph.children(parent)) {
			if (child.value() == '1') {
				if (breaks_left > 0 && time % 2 == 0 && times[breaks_left -1][child] > time + 1) {
					times[breaks_left - 1][child] = time + 1;
					queue.push({
							.index = child,
							.breaks_left = breaks_left - 1,
							.time = time + 1
							});
				}
			}
			else if (times[breaks_left][child] > time + 1) {
				times[breaks_left][child] = time + 1;
				queue.push({
						.index = child,
						.breaks_left = breaks_left,
						.time = time + 1
						});
			}
		}
	}
	std::cout << -1;
}
