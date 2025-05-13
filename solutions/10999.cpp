#include "../modules/LazySegmentTree.h"
#include "../modules/LazySum.h"
#include "../modules/InputRange.h"
#include "../modules/FastIO.h"
#include <cstdint>
#include <iostream>
int main() {
	FastIO();
	size_t n, m, k;
	std::cin >> n >> m >> k;

	LazySegmentTree<LazySum<int64_t>> tree(InputRange<int64_t>(n));

	for (size_t i = 0; i < m + k; i++) {
		char type;
		std::cin >> type;

		switch(type) {
			case '1': {
				size_t b, c;
				int64_t d;
				std::cin >> b >> c >> d;

				tree.update(b - 1, c, [d](LazySum<int64_t>& val) {
						val += d;
						});
				break;
		  	}
			case '2': {
				size_t b, c;
				std::cin >> b >> c;

				std::cout << tree.sum(b - 1, c) << '\n';
		  	}
		}
	}
}
