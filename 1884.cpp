#include "modules/ListGraph.h"
#include "modules/Matrix.h"
#include "modules/Types.h"
#include "modules/FastIO.h"

#include <iostream>
#include <limits>
#include <queue>
#include <utility>

struct Data {
	int length;
	int cost;

	bool operator<(const Data& other) const {
		if (this->length > other.length) return true; // Reverse

		return this->cost > other.cost;
	}
};

int main() {
	FastIO();
	int k;
	std::cin >> k;

	int n, r;
	std::cin >> n >> r;

	ListGraph<None, Data> graph(n);

	for(int i = 0; i < r; i++) {
		int s, d, l, t;
		std::cin >> s >> d >> l >> t;

		graph.connect(s - 1, d - 1, {.length = l, .cost = t});
	}

	Matrix<int> length(n, k + 1, std::numeric_limits<int>::max());

	for (int i = 0; i <= k; i++)
		length[0][i] = 0;

	std::priority_queue<std::pair<int, Data>> queue;

	queue.push({0, {0, 0}});

	while (!queue.empty()) {
		std::pair<int, Data> temp = queue.top();
		queue.pop();

		int parent = temp.first;
		Data edge = temp.second;

		if (length[parent][edge.cost] < edge.length) continue;

		graph.forEachChild(parent, [parent, edge, k, &length, &queue](int child, None& vertex, Data& data) {
				if (edge.cost + data.cost > k) return;

				if (length[child][edge.cost + data.cost] <= edge.length + data.length) return;

				length[child][edge.cost + data.cost] = edge.length + data.length;
				queue.emplace(child, Data {edge.length + data.length, edge.cost + data.cost});
				});
	}

	int min = std::numeric_limits<int>::max();

	for (int cost = 0; cost <= k; cost++)
		min = std::min(min, length[n - 1][cost]);

	if (min == std::numeric_limits<int>::max())
		std::cout << -1;
	else
		std::cout << min;
}
