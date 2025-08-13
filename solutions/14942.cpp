#include "FastIO2.h"
#include "ListGraph2.h"
#include "TreeWrapper.h"
#include "Matrix.h"
#include "SparseTable.h"
#include <variant>

template <typename T>
void set_distances(T& tree, uint parent) {
	for (auto child: tree.children(parent)) {
		tree[child] = child.edge();
		set_distances(tree, child);
	}
}

int main() {
	uint n;
	Fast::cin >> n;

	auto energies = Fast::cin.to_vec<int>(n);

	ListGraph<int, int> graph(n);

	for (uint i = 0; i < n - 1; i++) {
		uint a, b;
		int c;
		Fast::cin >> a >> b >> c;
		graph.connect(a - 1, b - 1, c);
		graph.connect(b - 1, a - 1, c);
	}

	TreeWrapper tree(graph, 0);

	set_distances(tree, 0);

	SparseTable parents(n, n - 1, [&tree](uint x) {
			return tree.parent(x);
			});

	Matrix<int> distances(n, parents.values.size().y);

	for (uint i = 0; i < n; i++)
		distances(i, 0) = tree[i];

	for (int pow = 1; pow < distances.size().y; pow ++) {
		for (uint i = 0; i < n; i++)
			distances(i, pow) = distances(i, pow - 1) + distances(parents.values(i, pow - 1), pow - 1);
	}

	for (uint i = 0; i < n; i++) {
		uint ptr = i;
		int accum_distance = 0;

		for (int upper_bound = distances.size().y - 1; upper_bound >= 0; upper_bound --) {
			int temp = accum_distance + distances(ptr, upper_bound);
			if (temp <= energies[i]) {
				accum_distance = temp;
				ptr = parents.values(ptr, upper_bound);
			}
		}

		std::cout << ptr + 1 << '\n';
	}
}
