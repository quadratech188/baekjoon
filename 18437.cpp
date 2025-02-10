#include <iostream>
#include <stack>

#include "modules/ListGraph.h"
#include "modules/Types.h"
#include "modules/LazySegmentTree.h"
#include "modules/LazyFill.h"
#include "modules/FastIO.h"

std::vector<Segment> segtree_indices;
int segtree_index = 0;

void calculate_indices(ListGraph<int, None>& graph, int parent) {
	int prev_index = segtree_index;
	segtree_index++;
	for (auto child: graph.children(parent))
		calculate_indices(graph, child);

	segtree_indices[parent] = Segment(prev_index + 1, segtree_index);
}

int main() {
	// FastIO();
	int n;
	std::cin >> n;

	ListGraph<int, None> graph(n);

	int garbage;
	std::cin >> garbage;

	for (int i = 1; i < n; i++) {
		int parent;
		std::cin >> parent;
		graph.connect(parent - 1, i);
	}

	segtree_indices.resize(n);

	calculate_indices(graph, 0);

	LazySegmentTree<LazyFill> tree(n, LazyFill::on());

	bool dir_down = true;

	int m;
	std::cin >> m;

	for (int i = 0; i < m; i++) {
		int type, index;
		std::cin >> type >> index;

		Segment children = segtree_indices[index - 1];

		switch(type) {
			case 1:
				if (children.size() == 0) continue;
				tree.update(children,
						[](LazyFill& fill) {fill.fill();});
				break;
			case 2:
				if (children.size() == 0) continue;
				tree.update(children,
						[](LazyFill& fill) {fill.unfill();});
				break;
			case 3:
				std::cout << (children.size() == 0? 0: tree.sum(children).count()) << '\n';
		}
	}
}
