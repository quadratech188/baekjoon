#include "FastIO2.h"
#include "LazySum.h"
#include "LazySegmentTree.h"
#include <iostream>
#include <vector>

int main() {
	uint n;
	Fast::cin >> n;

	std::vector<int64_t> values(n);

	int64_t prev = 0;
	for (uint i = 0; i < n; i++) {
		int a;
		Fast::cin >> a;
		values[i] = a - prev;
		prev = a;
	}

	LazySegmentTree<LazySum<int64_t>> tree(values);

	uint q;
	Fast::cin >> q;
	for (uint i = 0; i < q; i++) {
		char type;
		Fast::cin >> type;
		if (type == '1') {
			uint l, r;
			Fast::cin >> l >> r;
			l --;
			tree.update(l, r, [](LazySum<int64_t>& x) {
					x += 1;
					});
			if (r == n) continue;
			int len = r - l;
			tree.update(r, [len](LazySum<int64_t>& x) {
					x += - len;
					});
		}
		else {
			uint x;
			Fast::cin >> x;
			std::cout << tree.sum(0, x, [](LazySum<int64_t> const& x) {
					return x.extract();
					}) << '\n';
		}
	}
}
