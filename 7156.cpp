#include "modules/LazySegmentTree.h"
#include "modules/LazyTypes.h"
#include "modules/FastIO.h"

#include <algorithm>
#include <iomanip>
#include <ios>
#include <iostream>

enum BoundaryType {
	BEGIN,
	END
};

struct Boundary {
	Boundary(int x1, int x2, int y, BoundaryType type):
		segment(x1, x2), y(y), type(type) {}
	Segment segment;
	int y;
	BoundaryType type;

	bool operator<(const Boundary& other) {
		return y < other.y;
	}
};

bool loop() {
	int n;
	std::cin >> n;
	if (n == 0) return false;

	std::vector<Boundary> boundaries;
	boundaries.reserve(2 * n);

	for (int i = 0; i < n; i++) {
		int x1, y1, x2, y2;
		std::cin >> x1 >> y1 >> x2 >> y2;
		x1 += 1000;
		x2 += 1000;
		boundaries.emplace_back(x1, x2, y1, BEGIN);
		boundaries.emplace_back(x1, x2, y2, END);
	}

	std::sort(boundaries.begin(), boundaries.end());

	LazySegmentTree<LazyFill> tree(2000);

	int sum = 0;
	int last_y;
	for (Boundary& boundary: boundaries) {
		sum += (boundary.y - last_y) * tree.root().sum();
		tree.update(boundary.segment, [](LazyFill& val) {val.fill();});
		last_y = boundary.y;
	}

	std::cout << sum << '\n';

	return true;
}

int main() {
	FastIO();
	std::cout << std::fixed << std::setprecision(2);
	while (loop()) {}
}
