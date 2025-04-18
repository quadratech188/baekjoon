#include "modules/SegmentTree.h"
#include <iostream>
#include <vector>
#include <limits>

struct Child {
	int index;
	int cost;
};

static const int N = 100000;

std::vector<Child> neighbors[N];
int totalCosts[N];
int heavyEdges[N];

void calculateCosts(int root, int size) {

	int max = 0;
	int maxIndex;

	totalCosts[root] = 1; // Mark as used

	for (auto& child: neighbors[root]) {
		calculateCosts(child.index, size);
	}
}

void setHeavyEdges(int root, int size) {
	if (neighbors[root].empty()) return;

	int maxIndex;
	int max;

	for (auto& child: neighbors[root]) {
		if (totalCosts[child.index] + child.cost > max) {
			max = totalCosts[child.index] + child.cost;
			maxIndex = child.index;
		}
	}

	heavyEdges[root] = maxIndex;
}

int main() {
	int n;
	std::cin >> n;

	for (int i = 0; i < n - 1; i++) {
		int u, v, w;
		std::cin >> u >> v >> w;

		neighbors[v].push_back({
				.index = u,
				.cost = w
				});
		neighbors[u].push_back({
				.index = v,
				.cost = w
				});
	}

	calculateCosts(0, n);

	for (int i = 0; i < n; i++) setHeavyEdges(i, n);
}
