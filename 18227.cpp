#include <iostream>

#include "modules/ListGraph.h"
#include "modules/TreeWrapper.h"
#include "modules/Types.h"
#include "modules/SegmentTree.h"
#include "modules/FastIO.h"

std::vector<Segment> segtree_indices;
std::vector<int> depths;
int segtree_index = 0;

template <typename G>
void calculate_indices(G&& graph, int parent, int depth) {
	int prev_index = segtree_index;
	segtree_index++;
	for (auto child: graph.children(parent))
		calculate_indices(graph, child, depth + 1);

	segtree_indices[parent] = Segment(prev_index, segtree_index);
	depths[parent] = depth;
}

int main() {
	FastIO();
	int n, c;
	std::cin >> n >> c;

	ListGraph<None, None> graph(n);

	for (int i = 0; i < n - 1; i++) {
		int a, b;
		std::cin >> a >> b;
		graph.connect(a - 1, b - 1);
		graph.connect(b - 1, a - 1);
	}

	segtree_indices.resize(n);
	depths.resize(n);

	calculate_indices(TreeWrapper(graph, c - 1), c - 1, 1);

	SegmentTree<long long int> salaries(n);

	int q;
	std::cin >> q;

	for (int i = 0; i < q; i++) {
		int type, a;
		std::cin >> type >> a;

		switch(type) {
			case 1:
				salaries.update(segtree_indices[a - 1].start, [](long long int& val) {val++;});
				break;
			case 2:
				std::cout << salaries.sum(segtree_indices[a - 1]) * depths[a - 1] << '\n';
		}
	}
}
