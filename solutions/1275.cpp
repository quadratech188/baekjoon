#include "modules/FastIO.h"
#include "modules/SegmentTree.h"
#include "modules/InputIterator.h"
#include <iterator>

int main() {
	FastIO();
	int n, q;
	std::cin >> n >> q;

	SegmentTree<int> tree(n, std::istream_iterator<int>(std::cin));

	for (int i = 0; i < q; i++) {
		int x, y, a, b;
		std::cin >> x >> y >> a >> b;
		if (y < x)
			std::swap(x, y);
		std::cout << tree.sum(x - 1, y) << '\n';
		tree.update(a - 1, [b](int& val) {val = b;});
	}
}
