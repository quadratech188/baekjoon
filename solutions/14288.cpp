#include <iostream>
#include <stack>

#include "modules/ListGraph.h"
#include "modules/Types.h"
#include "modules/LazySegmentTree.h"
#include "modules/FastIO.h"

std::vector<Segment> segtree_indices;
int segtree_index = 0;

struct Data {
	Data():
		_value(0), delta(0) {}
	Data(int value):
		_value(value), delta(0) {}

	int _value;
	int delta;
	Data operator+(const Data& other) {
		return Data(value() + other.value());
	}

	void resolve(Data& left, Data& right) {
		left.delta += delta;
		right.delta += delta;
		_value += delta;
		delta = 0;
	}

	int value() const {
		return _value + delta;
	}
};

void calculate_indices(ListGraph<int, None>& graph, int parent) {
	int prev_index = segtree_index;
	segtree_index++;
	for (auto child: graph.children(parent))
		calculate_indices(graph, child);

	segtree_indices[parent] = Segment(prev_index, segtree_index);
}

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;

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

	LazySegmentTree<Data> up(n), down(n);

	bool dir_down = true;

	for (int i = 0; i < m; i++) {
		int type;
		int a, x;
		std::cin >> type;

		switch(type) {
			case 1:
				std::cin >> a >> x;
				if (dir_down)
					down.update(segtree_indices[a - 1],
						[x](Data& val) {val.delta += x;});
				else
					up.update(segtree_indices[a - 1].start + 1, n,
						[x](Data& val) {val.delta += x;});
				break;
			case 2:
				std::cin >> a;
				std::cout << up.at(segtree_indices[a - 1].end - 1).value()
					- up.at(segtree_indices[a - 1].start).value()
					+ down.at(segtree_indices[a - 1].start).value() << '\n';
				break;
			case 3:
				dir_down = !dir_down;
		}
	}
}
