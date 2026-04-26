#include "FastIO2.h"
#include "UnionFind.h"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <map>
#include <vector>

struct Edge {
	uint u, v;
	int w;

	bool operator<(Edge const& other) const {
		if (w != other.w) return w < other.w;
		return std::make_pair(u, v) < std::make_pair(other.u, other.v);
	}
};

int main() {
	std::cout << "NO\n";

	uint n, m;
	Fast::cin >> n >> m;

	std::vector<Edge> edges;
	std::map<Edge, int> indices;

	for (uint i = 0; i < m; i++) {
		uint u, v;
		int w;
		Fast::cin >> u >> v >> w;
		Edge edge = {u - 1, v - 1, w};
		edges.push_back(edge);
		indices[edge] = i + 1;
	}

	UnionFind uf(n);

	std::sort(edges.begin(), edges.end());

	int max_weight = 0;
	uint64_t sum = 0;

	for (auto edge: edges) {
		if (uf.connected(edge.u, edge.v)) continue;
		uf.connect(edge.u, edge.v);
		max_weight = edge.w;
		sum += edge.w;
	}
	std::vector<Edge> small_edges;
	for (auto edge: edges) {
		if (edge.w > max_weight) break;
		small_edges.push_back(edge);
	}

	std::reverse(small_edges.begin(), small_edges.end());

	UnionFind uf2(n);

	std::vector<int> results;

	uint64_t new_sum = 0;

	for (auto edge: small_edges) {
		if (uf2.connected(edge.u, edge.v)) continue;
		uf2.connect(edge.u, edge.v);
		new_sum += edge.w;
		results.push_back(indices[edge]);
	}

	if (sum == new_sum) {
		std::cout << "NO\n";
		return 0;
	}
	else {
		std::cout << "YES\n";
		for (int i: results) {
			std::cout << i << '\n';
		}
	}
}
