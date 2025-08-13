#include "FastIO2.h"
#include "SegmentTree2.h"
#include <iostream>
#include <numeric>
#include <ranges>

void loop() {
	uint n, m;
	Fast::cin >> n >> m;

	SegmentTree<int> tree(std::views::iota(0u, n + m) | std::views::transform([n](uint i) {
				return i < n? 1 : 0;
				}));

	std::vector<uint> indices(n);
	std::iota(indices.begin(), indices.end(), 0);

	for (uint i = 0; i < m; i++) {
		uint x;
		Fast::cin >> x;
		x = n - x;
		std::cout << tree.sum(indices[x], tree.size()) - 1 <<  ' ';

		tree.update(indices[x], [](int& x) {x = 0;});
		tree.update(n + i, [](int& x) {x = 1;});

		indices[x] = n + i;
	}
	std::cout << '\n';
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
