#include "../modules/SegmentTree.h"
#include "../modules/InputRange.h"
#include "../modules/FastIO.h"
#include "../modules/FastIO2.h"
#include <cstdint>
#include <sys/types.h>

int main() {
	FastIO();
	uint n, m, k;
	Fast::cin >> n >> m >> k;

	SegmentTree<int64_t> tree(n, [](int64_t& val) {Fast::cin >> val;});

	for (uint i = 0; i < m + k; i++) {
		char a;
		Fast::cin >> a;

		switch(a) {
			case '1': {
				size_t b;
				int64_t c;

				Fast::cin >> b >> c;

				tree.update(b - 1, [c](int64_t& val) {
						val = c;
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
