#include "../modules/FastIO2.h"
#include "../modules/UnionFind.h"
#include <algorithm>

struct Edge {
	int distance;
	uint x, y;

	bool operator<(Edge const& other) const {
		return distance < other.distance;
	}
};

int main() {
	uint n;
	Fast::cin >> n;

	std::vector<std::pair<int, uint>> xs(n),ys(n), zs(n);

	for (uint i = 0; i < n; i++) {
		int x, y, z;
		Fast::cin >> x >> y >> z;

		xs[i] = {x, i};
		ys[i] = {y, i};
		zs[i] = {z, i};
	}

	std::ranges::sort(xs);
	std::ranges::sort(ys);
	std::ranges::sort(zs);

	std::vector<Edge> edges;

	for (uint i = 0; i < n - 1; i++) {
		edges.emplace_back(xs[i + 1].first - xs[i].first, xs[i + 1].second, xs[i].second);
		edges.emplace_back(ys[i + 1].first - ys[i].first, ys[i + 1].second, ys[i].second);
		edges.emplace_back(zs[i + 1].first - zs[i].first, zs[i + 1].second, zs[i].second);
	}

	std::sort(edges.begin(), edges.end());

	UnionFind uf(n);

	uint counter = 0;
	int cost = 0;
	for (Edge edge: edges) {
		if (uf.connected(edge.x, edge.y)) continue;

		counter++;
		uf.connect(edge.x, edge.y);
		cost += edge.distance;

		if (counter == n - 1) break;
	}

	std::cout << cost;
}
