#include <algorithm>
#include <iostream>
#include "modules/SegmentTree.h"
#include "modules/Matrix.h"
#include "modules/Types.h"
struct Data {
	int value;
	Data(int value): value(value) {}
	Data(): value(0) {}
	operator int() {
		return value;
	}

	Data operator+(const Data& other) const {
		return Data(std::max(value, other.value));
	}

	bool operator<(const Data& other) const {
		return value < other.value;
	}

	void update(int action) {
		value = action;
	}
};

struct Tree: public SegmentTree<Data, int> {
	Tree(int n): SegmentTree(n) {}

	Tree operator+(const Tree& other) {
		Tree result(length);

		for (int i = 0; i < values.size(); i++)
			result.values[i] = std::max(values[i], other.values[i]);

		return result;
	}
};

struct Wrapper {
	Wrapper(): tree(1) {}
	Wrapper(Tree tree): tree(tree) {}
	Tree tree;

	Wrapper operator+(const Wrapper& other) {
		return {tree + other.tree};
	}

	void update(std::pair<int, int> action) {
		tree.update(action.first, action.second);
	}
};

int main() {
	int n, m;
	std::cin >> n >> m;

	Matrix<int> data(1500, 1500);

	for (int i = 0; i < n; i++) {
		int y1, x1, y2, x2;
		std::cin >> y1 >> x1 >> y2 >> x2;
		for (int x = x1; x < x2; x++) {
			data[x][y1] += 1;
			data[x][y2] -= 1;
		}
	}

	SegmentTree<Wrapper, std::pair<int, int>> tree(1500, Wrapper{Tree(1500)});
	for (int x = 0; x < 1500; x++) {
		int curr = 0;
		for (int y = 0; y < 1500; y++) {
			curr += data[x][y];
			tree.update(x, {y, curr});
		}
	}


	for (int i = 0; i < m; i++) {
		size_t y1, x1, y2, x2;
		std::cin >> y1 >> x1 >> y2 >> x2;

		std::cout << tree.query({x1, x2}).tree.query({y1, y2}) << '\n';
	}
}
