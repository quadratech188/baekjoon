#include "../modules/LazySegmentTree.h"
#include "../modules/LazySum.h"
#include "../modules/InputRange.h"
#include "../modules/FastIO.h"
#include "../modules/FastIO2.h"
#include <cstdint>
#include <iostream>

int main() {
	FastIO();
	size_t n, m, k;
	Fast::cin >> n >> m >> k;

	LazySegmentTree<LazySum<int64_t>
		::with_length<uint>> tree(InputRange<int64_t, Fast::istream>(n, Fast::cin));

	for (size_t i = 0; i < m + k; i++) {
		char type;
		Fast::cin >> type;

		switch(type) {
			case '1': {
				size_t b, c;
				int64_t d;
				Fast::cin >> b >> c >> d;

				tree.update(b - 1, c, [d](decltype(tree)::value_type& val) {
						val += d;
						});
				break;
		  	}
			case '2': {
				size_t b, c;
				Fast::cin >> b >> c;

				std::cout << tree.sum(b - 1, c) << '\n';
		  	}
		}
	}
}
