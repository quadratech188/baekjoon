#include "FastIO2.h"
#include "SegmentTree2.h"
#include "OverloadedPlus.h"
#include <algorithm>
#include <iostream>

int main() {
	uint n, d;
	Fast::cin >> n >> d;

	SegmentTree<maxxer<int64_t>> tree(Fast::cin.to_range<int>(n));

	for (uint i = 1; i < d; i++) {
		int64_t max = std::max(tree.sum(0, i).val(), 0l);
		tree.update(i, [max](maxxer<int64_t>& x) {x = x.val() + max;});
	}
	for (uint i = d; i < n; i++) {
		int64_t max = std::max(tree.sum(i - d, i).val(), 0l);
		tree.update(i, [max](maxxer<int64_t>& x) {x = x.val() + max;});
	}

	std::cout << tree.root().val();
}
