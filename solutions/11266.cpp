#include "FastIO2.h"
#include "ListGraph2.h"
#include <variant>

// https://en.wikipedia.org/wiki/Biconnected_component#Linear_time_depth-first_search

ListGraph<std::monostate, std::monostate> graph;

std::vector<bool> visited;
std::vector<uint> depth, low, parent;

std::vector<uint> articulations;

void GetArticulationPoints(uint i, uint d) {
	visited[i] = true;
	depth[i] = d;
	low[i] = d;
	uint childCount = 0;
	bool isArticulation = false;

	for (auto ni: graph.children(i)) {
		if (!visited[ni]) {
			parent[ni] = i;
			GetArticulationPoints(ni, d + 1);
			childCount = childCount + 1;
			if (low[ni] >= depth[i])
				isArticulation = true;
			low[i] = std::min(low[i], low[ni]);
		}
		else if (ni != parent[i])
			low[i] = std::min(low[i], depth[ni]);
	}
	if ((parent[i] != i && isArticulation) || (parent[i] == i && childCount > 1))
		articulations.push_back(i);
}

int main() {
	uint v, e;
	Fast::cin >> v >> e;

	graph.resize(v);
	visited.resize(v);
	depth.resize(v);
	low.resize(v);
	parent.resize(v);

	for (uint i = 0; i < e; i++) {
		uint a, b;
		Fast::cin >> a >> b;
		graph.connect(a - 1, b - 1);
		graph.connect(b - 1, a - 1);
	}

	for (uint i = 0; i < v; i++) {
		if (!visited[i]) {
			parent[i] = i;
			GetArticulationPoints(i, 0);
		}
	}

	std::ranges::sort(articulations);
	std::cout << articulations.size() << '\n';
	for (auto val: articulations)
		std::cout << val + 1 << ' ';
}
