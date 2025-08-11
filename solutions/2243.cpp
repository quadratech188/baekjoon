#include "FastIO2.h"
#include "SegmentTree2.h"
#include <algorithm>
#include <iostream>
#include <ranges>

int main() {
	uint n;
	Fast::cin >> n;

	SegmentTree<int> tree(1'000'000);

	for (uint i = 0; i < n; i++) {
		char type;
		Fast::cin >> type;
		if (type == '1') {
			int b;
			Fast::cin >> b;

			int result =  *std::ranges::partition_point(std::views::iota(1u, 1'000'001u), [&tree, b](uint x) {
					return tree.sum(0, x) < b;
					});
			tree.update(result - 1, [](int& x) {x --;});
			std::cout << result << '\n';
		}
		else {
			uint b;
			int c;
			Fast::cin >> b >> c;
			tree.update(b - 1, [c](int& x) {x += c;});
		}
	}
}
