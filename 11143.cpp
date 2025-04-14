#include "modules/FastIO.h"
#include "modules/SegmentTree.h"
#include <iostream>

void loop() {
	int b, p, q;
	std::cin >> b >> p >> q;

	SegmentTree<int> tree(b, 0);

	for (int _ = 0; _ < p + q; _++) {
		char type;
		std::cin >> type;
		switch(type) {
			case 'P': {
				int i, a;
				std::cin >> i >> a;
				tree.update(i - 1, [a](int& val) {val += a;});
				break;
			}
			case 'Q': {
				int i, j;
				std::cin >> i >> j;
				std::cout << tree.sum(i - 1, j) << '\n';
			}
		}
	}
}

int main() {
	FastIO();
	int k;
	std::cin >> k;

	for (int i = 0; i < k; i++)
		loop();
}

