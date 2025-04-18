#include "modules/FastIO.h"
#include "modules/ListGraph.h"
#include "modules/Input.h"
#include <queue>

std::vector<int> population, C, R, X;

int main() {
	FastIO();
	int n;
	std::cin >> n;
	ListGraph<int, int> graph(n);

	for (int i = 0; i < n - 1; i++) {
		int u, v, w;
		std::cin >> u >> v >> w;
		graph.connect(u - 1, v - 1, w);
		graph.connect(v - 1, u - 1, w);
	}

	std::vector<int> T = Input::toVec<int>(n);

	population = std::vector<int>(n, 0);
	C = std::vector<int>(n);
	R = std::vector<int>(n);
	X = std::vector<int>(n);

	for (int i = 0; i < n; i++) {
		std::cin >> C[i] >> R[i] >> X[i];
	}

	std::queue<int> queue;
	while (!queue.empty()) {
		int parent = queue.front();
		queue.pop();

		for (auto& it: graph.children(parent)) {
			if (it.edge() > R[parent]) continue;

			int child = it.index();
			population[child] += X[parent];
		}
	}
}
