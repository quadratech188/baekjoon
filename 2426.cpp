#include "modules/LazySegmentTree.h"
#include "modules/Input.h"
#include <iostream>

int main() {
	int n, m;
	std::cin >> n >> m;

	std::vector<int> heights = Input::toVec<int>(n);

	LazySegmentTree<int> tree(heights);
}
