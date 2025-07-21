#include "../modules/UnionFind.h"
#include "../modules/FastIO2.h"
#include <algorithm>

struct Edge {
	uint parent;
	uint child;
	uint cost;

	bool operator<(Edge const& other) const {
		return cost < other.cost;
	}
};

int main() {
	uint n, m;
	Fast::cin >> n >> m;

	UnionFind uf(n);

	std::vector<Edge> edges(m);
	
	for (auto& val: edges) {
		Fast::cin >> val.parent >> val.child >> val.cost;
		val.parent --;
		val.child --;
	}

	std::sort(edges.begin(), edges.end());

	uint total_cost = 0;

	uint counter = 0;
	for (auto [parent, child, cost]: edges) {
		if (counter == n - 2) break;

		if (uf.connected(parent, child)) continue;

		uf.connect(parent, child);
		total_cost += cost;
		counter++;
	}

	std::cout << total_cost;
}
