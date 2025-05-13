#include "../modules/LazySegmentTree.h"
#include "../modules/LazySum.h"
#include "../modules/InputRange.h"
#include "../modules/FastIO.h"
#include <cstdint>
#include <iostream>
int main() {
	FastIO();
	size_t n;
	std::cin >> n;

	LazySegmentTree<LazySum<int64_t>> tree(InputRange<int64_t>(n));

	size_t m;
	std::cin >> m;
	for (size_t i = 0; i < m; i++) {
		char type;
		std::cin >> type;

		switch(type) {
			case '1': {
				size_t i, j;
				int64_t k;
				std::cin >> i >> j >> k;

				tree.update(i - 1, j, [k](LazySum<int64_t>& val) {
						val += k;
						});
				break;
		  	}
			case '2': {
				size_t x;
				std::cin >> x;

				std::cout << tree.at(x - 1) << '\n';
		  	}
		}
	}
}
