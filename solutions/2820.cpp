#include <iostream>
#include <stack>

#include "modules/ListGraph.h"
#include "modules/Types.h"
#include "modules/LazySegmentTree.h"
#include "modules/FastIO.h"

std::vector<std::pair<int, int>> segtree_indices;
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

	segtree_indices[parent] = std::make_pair(prev_index, segtree_index);
}

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;

	ListGraph<int, None> graph(n);

	std::cin >> graph[0];

	for (int i = 1; i < n; i++) {
		int parent;
		std::cin >> graph[i] >> parent;
		graph.connect(parent - 1, i);
	}

	segtree_indices.resize(n);

	calculate_indices(graph, 0);

	std::vector<int> segtree_values(n);
	for (int i = 0; i < n; i++) {
		segtree_values[segtree_indices[i].first] = graph[i];
	}

	LazySegmentTree<Data> salaries(segtree_values);

	for (int i = 0; i < m; i++) {
		char type;
		int a, x;
		std::cin >> type;
		switch(type) {
			case 'p':
				std::cin >> a >> x;
				if (segtree_indices[a - 1].second - segtree_indices[a - 1].first == 1) break;
				salaries.update(segtree_indices[a - 1].first + 1, segtree_indices[a - 1].second,
						[x](Data& val) {val.delta += x;});
				break;
			case 'u':
				std::cin >> a;
				std::cout << salaries.at(segtree_indices[a - 1].first).value() << '\n';
		}
	}
}
