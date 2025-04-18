#include <iostream>
#include <vector>
#include "modules/Bounds2.h"
#include "modules/Input.h"
#include "modules/Compress.h"
#include "modules/LazySegmentTree.h"

Compress<int> x, y;

struct Data {
	Data():
		count(0), length(1), delta(0) {}
	Data(int count, int length):
		count(count), length(length), delta(0) {}

	int count;
	int length;
	int delta;

	int value() const {
		return count + delta;
	}

	Data operator+(const Data& other) {
		if (value() > other.value())
			return Data(value(), length);
		else if (value() == other.value())
			return Data(value(), length + other.length);
		else
		 	return Data(other.value(), other.length);
	}

	void resolve(Data& l, Data& r) {
		l.delta += delta;
		r.delta += delta;
		delta = 0;
	}
};

struct Edge {
	int delta;
	int x1;
	int x2;
	int y;

	bool operator<(Edge const& other) {
		return y < other.y;
	}
};

int main() {
	int n;
	std::cin >> n;

	std::vector<Range2> fences = Input::toVec<Range2>(n);

	std::vector<int> x_vec;

	std::vector<int> y_vec;
	std::vector<Edge> boundaries;

	for (Range2& r: fences) {
		x_vec.push_back(r.x1());
		x_vec.push_back(r.x2());
		boundaries.push_back({1, r.x1(), r.x2(), r.y1()});
		boundaries.push_back({-1, r.x1(), r.x2(), r.y2()});
	}

	std::sort(boundaries.begin(), boundaries.end());
	std::sort(x_vec.begin(), x_vec.end());

	Compress<int> x_compresssor(x_vec);

	LazySegmentTree<Data> tree(x_compresssor.size() - 1);

	int prev_y = 0;

	int max;
	int max_count;

	for (Edge edge: boundaries) {
		if (tree.root().value() > max) {
			max = tree.root().value();
			max_count = tree.root().length;
		}
		if (tree.root().value() == max) {
			max_count += tree.root().length;
		}

		tree.update(x_compresssor.compress(edge.x1),
				x_compresssor.compress(edge.x2),
				[&edge](Data& val) {
				val.delta += edge.delta;
				});
		prev_y = edge.y;
	}

	std::cout << max << ' ' << max_count;
}
