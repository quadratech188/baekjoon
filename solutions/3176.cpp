#include "../modules/FastIO2.h"
#include "../modules/TreeWrapper.h"
#include "../modules/Tree.h"
#include "../modules/ListGraph2.h"
#include "../modules/SparseTable.h"
#include <limits>

template <Tree T>
void setup(T& graph, std::vector<int>& lengths, uint parent) {
	for (auto child: graph.children(parent)) {
		graph[child] = graph[parent] + 1;
		lengths[child] = child.edge();
		setup(graph, lengths, child);
	}
}

struct Data {
	int max;
	int min;

	Data():
		max(std::numeric_limits<int>::min()),
		min(std::numeric_limits<int>::max()) {}

	Data(int val):
		max(val), min(val) {}

	Data(int max, int min):
		max(max), min(min) {}
	Data operator+(Data const& other) const {
		return {
			std::max(max, other.max),
			std::min(min, other.min)
		};
	}
};

int main() {
	uint n;
	Fast::cin >> n;

	ListGraph<uint, int> graph(n);

	for (uint i = 0; i < n - 1; i++) {
		uint a, b;
		int c;
		Fast::cin >> a >> b >> c;
		graph.connect(a - 1, b - 1, c);
		graph.connect(b - 1, a - 1, c);
	}

	TreeWrapper tree(graph, 0);

	std::vector<int> lengths(n);

	setup(tree, lengths, 0);

	SparseTable parent_table(n, n - 1, [&tree](uint i) {
			return tree.parent(i);
			});

	Matrix<Data> min_table(parent_table.values.size().x, parent_table.values.size().y);

	for (uint i = 0; i < n; i++)
		min_table(i, 0) = {lengths[i], lengths[i]};

	for (size_t row = 1; row < min_table.size().y; row++) {
		for (size_t i = 0; i < n; i++) {
			size_t intermediate = parent_table.values(i, row - 1);
			min_table(i, row) = min_table(i, row - 1) + min_table(intermediate, row - 1);
		}
	}

	auto after = [&min_table, &parent_table](size_t index, int iterations) {
		size_t ptr = index;
		Data result;

		for (size_t i = 0; iterations > 0; i++) {
			if (iterations & 1) {
				result = result + min_table(ptr, i);
				ptr = parent_table.values(ptr, i);
			}
			iterations >>= 1;
		}

		return result;
	};


	auto lca = [&tree, &parent_table](uint a, uint b) {
		if (tree[a] > tree[b])
			a = parent_table.after(a, tree[a] - tree[b]);
		else
		 	b = parent_table.after(b, tree[b] - tree[a]);

		return parent_table.follow_until_same(a, b);
	};

	uint k;
	Fast::cin >> k;

	for (uint i = 0; i < k; i++) {
		uint d, e;
		Fast::cin >> d >> e;
		d--;
		e--;
		uint lc = lca(d, e);

		Data result = after(d, tree[d] - tree[lc]) + after(e, tree[e] - tree[lc]);
		std::cout << result.min << ' ' << result.max << '\n';
	}
}
